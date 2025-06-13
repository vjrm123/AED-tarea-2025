import json
from Avl import AVLIndex
from tkinter import messagebox

class BaseDeDatosFiltrado:
    def __init__(self):
        self.tablas = {}
        from Disco import DISCOLBA  # Importación local para evitar dependencia circular
        self.disco = DISCOLBA()

    def cargar_datos(self, nombre_tabla, datos):
        if nombre_tabla not in self.tablas:
            columnas = list(datos[0].keys())
            tipos = {col: type(datos[0][col]) for col in columnas}
            self.tablas[nombre_tabla] = {
                'columnas': columnas,
                'tipos': tipos,
                'indices': {col: AVLIndex() for col in columnas},
                'registros': {}
            }

        tabla = self.tablas[nombre_tabla]
        for registro in datos:
            id_registro = f"{nombre_tabla}_{len(tabla['registros']) + 1}"
            dato_str = json.dumps(registro)
            self.disco.guardar_dato(dato_str, id_registro)
            lbas = self.disco.mapa_ubicacion_fisica[id_registro]
            for col, valor in registro.items():
                tabla['indices'][col].insertar(valor, id_registro, lbas)
            tabla['registros'][id_registro] = registro

    def select(self, nombre_tabla, condiciones=None):
        if nombre_tabla not in self.tablas:
            return []

        resultados = []
        tabla = self.tablas[nombre_tabla]
        registros = tabla['registros']

        # Si no hay condiciones, retornar todo
        if not condiciones:
            for id_reg, reg in registros.items():
                resultados.append({
                    'registro': reg,
                    'id': id_reg,
                    'ubicacion_disco': self.disco.mapa_ubicacion_fisica[id_reg]
                })
            return resultados

        ids_validos = set()

        for i, (col, op, val) in enumerate(condiciones):
            if col not in tabla['indices']:
                continue

            # ✅ Validación de tipo dinámico
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
                            raise ValueError("Valor booleano inválido")
                    else:
                        val = tipo_esperado(val)
                except (ValueError, TypeError):
                    messagebox.showerror("Error", f"El valor '{val}' no coincide con el tipo esperado para '{col}' ({tipo_esperado.__name__})")
                    return []

            # Solo soportamos igualdad por ahora
            if op != '=':
                messagebox.showerror("Error", "Solo se permite el operador de igualdad (=)")
                return []

            # Buscar en índice AVL
            coincidencias = tabla['indices'][col].buscar(val)
            ids_actuales = {id_reg for id_reg, _ in coincidencias}

            if i == 0:
                ids_validos = ids_actuales
            else:
                ids_validos &= ids_actuales  # intersección

        # Construir resultados
        for id_reg in ids_validos:
            reg = registros[id_reg]
            resultados.append({
                'registro': reg,
                'id': id_reg,
                'ubicacion_disco': self.disco.mapa_ubicacion_fisica[id_reg]
            })

        return resultados
