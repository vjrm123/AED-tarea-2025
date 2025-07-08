from disco import DISCO
from Avl import AVLIndex
from estructura import empaquetar, desempaquetar

class BaseDeDatos:
    def __init__(self, disco=None):
        self.disco = disco if disco else DISCO()
        self.tablas = {}

    def definir_tabla(self, nombre_tabla, estructura):
        columnas = [col for col, _ in estructura]
        tipos = {
            col: (
                int if tipo == 'i' else
                float if tipo == 'd' else
                str
            )
            for col, tipo in estructura
        }

        self.tablas[nombre_tabla] = {
            'estructura': estructura,
            'columnas': columnas,
            'tipos': tipos,
            'indices': {col: AVLIndex() for col in columnas if col.lower() != "Index"},
            'registros': {}  
        }

    def cargar_datos(self, nombre_tabla, filas):
        tabla = self.tablas[nombre_tabla]
        estructura = tabla['estructura']

        for fila in filas:
            id_reg = f"{nombre_tabla}_{len(tabla['registros']) + 1}"
            binario = empaquetar(estructura, fila)
            self.disco.guardar_dato(binario, id_reg)
            ubicacion = self.disco.obtener_ubicacion(id_reg)

            for (col, _), val in zip(estructura, fila):
                tabla['indices'][col].insertar(val, id_reg, ubicacion)

            tabla['registros'][id_reg] = ubicacion

    def buscar_por_campo(self, nombre_tabla, columna, valor):
        tabla = self.tablas.get(nombre_tabla)
        if not tabla:
            return []

        estructura = tabla['estructura']
        tipo = tabla['tipos'].get(columna, str)

        if not valor.strip():
            return self._recuperar_todos(tabla)

        try:
            valor_convertido = tipo(valor)
        except:
            return []

        avl = tabla['indices'].get(columna)
        if not avl:
            return []

        coincidencias = avl.buscar(valor_convertido)
        resultados = []

        for id_reg, _ in coincidencias:
            data = self.disco.recuperar_dato(id_reg)
            if data:
                fila = desempaquetar(estructura, data)
                resultados.append({
                    "id": id_reg,
                    "registro": {col: val for (col, _), val in zip(estructura, fila)},
                    "ubicacion": self.disco.obtener_ubicacion(id_reg)
                })
        return resultados

    def _recuperar_todos(self, tabla):
        estructura = tabla['estructura']
        resultados = []

        for id_reg, ubicacion in tabla['registros'].items():
            data = self.disco.recuperar_dato(id_reg)
            if data:
                fila = desempaquetar(estructura, data)
                resultados.append({
                    "id": id_reg,
                    "registro": {col: val for (col, _), val in zip(estructura, fila)},
                    "ubicacion": ubicacion
                })
        return resultados
