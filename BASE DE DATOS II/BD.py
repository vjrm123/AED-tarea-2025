import json
from tkinter import messagebox
from Avl import AVLIndex
from Disco import DISCOLBA

class BaseDeDatosFiltrado:
    def __init__(self):
        self.tablas = {}
        self.disco = DISCOLBA()

    def cargar_datos(self, nombre_tabla, lista_registros):
        if nombre_tabla not in self.tablas:
            columnas = list(lista_registros[0].keys())
            tipos = {col: type(lista_registros[0][col]) for col in columnas}
            self.tablas[nombre_tabla] = {
                'columnas': columnas,
                'tipos': tipos,
                'indices': {col: AVLIndex() for col in columnas},
                'registros': {}
            }

        tabla = self.tablas[nombre_tabla]

        for registro in lista_registros:
            id_registro = f"{nombre_tabla}_{len(tabla['registros']) + 1}"

            # Guardar en disco con sector compartido
            self.disco.guardar_dato(registro, id_registro)
            ubicacion = self.disco.obtener_ubicacion(id_registro)  # [(lba, inicio, fin), ...]

            # Insertar en índices
            for col, valor in registro.items():
                tabla['indices'][col].insertar(valor, id_registro, ubicacion)

            tabla['registros'][id_registro] = registro

    def select(self, nombre_tabla, condiciones=None):
        if nombre_tabla not in self.tablas:
            return []

        tabla = self.tablas[nombre_tabla]
        registros = tabla['registros']

        if not condiciones:
            return [
                {
                    'registro': reg,
                    'id': id_reg,
                    'ubicacion_disco': self.disco.obtener_ubicacion(id_reg)
                }
                for id_reg, reg in registros.items()
            ]

        ids_validos = set()

        for i, (col, op, val) in enumerate(condiciones):
            if col not in tabla['indices']:
                continue

            tipo_esperado = tabla['tipos'].get(col)
            if tipo_esperado:
                try:
                    if tipo_esperado == bool:
                        val = str(val).strip().lower()
                        if val in ("true", "1"):
                            val = True
                        elif val in ("false", "0"):
                            val = False
                        else:
                            raise ValueError()
                    else:
                        val = tipo_esperado(val)
                except:
                    messagebox.showerror("Error", f"Tipo inválido para '{col}'")
                    return []

            if op != '=':
                messagebox.showerror("Error", "Solo se permite '=' por ahora")
                return []

            coincidencias = tabla['indices'][col].buscar(val)
            ids_actuales = {id_reg for id_reg, _ in coincidencias}

            if i == 0:
                ids_validos = ids_actuales
            else:
                ids_validos &= ids_actuales

        return [
            {
                'registro': tabla['registros'][id_reg],
                'id': id_reg,
                'ubicacion_disco': self.disco.obtener_ubicacion(id_reg)
            }
            for id_reg in ids_validos
        ]
