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

    def buscar_por_campo(self, nombre_tabla, columna, valor):
        if nombre_tabla not in self.tablas:
            return []

        tabla = self.tablas[nombre_tabla]
        tipo = tabla['tipos'].get(columna, str)

        try:
            valor = tipo(valor)
        except:
            return []

        # 👇 Si no existe el índice, lo construye dinámicamente
        if columna not in tabla['indices'] or tabla['indices'][columna] is None:
            from Avl import AVLIndex
            avl = AVLIndex()
            for id_registro, registro in tabla['registros'].items():
                ubicacion = self.disco.obtener_ubicacion(id_registro)
                avl.insertar(registro[columna], id_registro, ubicacion)
            tabla['indices'][columna] = avl

        avl = tabla['indices'][columna]
        coincidencias = avl.buscar(valor)

        return [
            {
                'registro': tabla['registros'][id_reg],
                'id': id_reg,
                'ubicacion_disco': self.disco.obtener_ubicacion(id_reg)
            }
            for id_reg, _ in coincidencias
        ]

