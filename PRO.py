import tkinter as tk
from tkinter import ttk, messagebox
import json

# -------------------- AVL --------------------
class NodoAVL:
    def __init__(self, clave, valores):
        self.clave = clave
        self.valores = valores  # lista de (id_registro, [LBAs])
        self.izquierda = None
        self.derecha = None
        self.altura = 1

class AVLIndex:
    def __init__(self):
        self.raiz = None

    def insertar(self, clave, id_registro, lbas):
        self.raiz = self._insertar(self.raiz, clave, id_registro, lbas)

    def _insertar(self, nodo, clave, id_registro, lbas):
        if not nodo:
            return NodoAVL(clave, [(id_registro, lbas)])
        if clave < nodo.clave:
            nodo.izquierda = self._insertar(nodo.izquierda, clave, id_registro, lbas)
        elif clave > nodo.clave:
            nodo.derecha = self._insertar(nodo.derecha, clave, id_registro, lbas)
        else:
            nodo.valores.append((id_registro, lbas))
            return nodo

        nodo.altura = 1 + max(self._altura(nodo.izquierda), self._altura(nodo.derecha))
        balance = self._balance(nodo)

        if balance > 1 and clave < nodo.izquierda.clave:
            return self._rotar_derecha(nodo)
        if balance < -1 and clave > nodo.derecha.clave:
            return self._rotar_izquierda(nodo)
        if balance > 1 and clave > nodo.izquierda.clave:
            nodo.izquierda = self._rotar_izquierda(nodo.izquierda)
            return self._rotar_derecha(nodo)
        if balance < -1 and clave < nodo.derecha.clave:
            nodo.derecha = self._rotar_derecha(nodo.derecha)
            return self._rotar_izquierda(nodo)

        return nodo

    def buscar(self, clave):
        return self._buscar(self.raiz, clave)

    def _buscar(self, nodo, clave):
        if not nodo:
            return []
        if clave == nodo.clave:
            return nodo.valores
        elif clave < nodo.clave:
            return self._buscar(nodo.izquierda, clave)
        else:
            return self._buscar(nodo.derecha, clave)

    def inorden(self):
        resultado = []
        self._inorden(self.raiz, resultado)
        return resultado

    def _inorden(self, nodo, resultado):
        if nodo:
            self._inorden(nodo.izquierda, resultado)
            resultado.extend(nodo.valores)
            self._inorden(nodo.derecha, resultado)

    def _altura(self, nodo):
        return nodo.altura if nodo else 0

    def _balance(self, nodo):
        return self._altura(nodo.izquierda) - self._altura(nodo.derecha)

    def _rotar_derecha(self, z):
        y = z.izquierda
        T3 = y.derecha
        y.derecha = z
        z.izquierda = T3
        z.altura = 1 + max(self._altura(z.izquierda), self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), self._altura(y.derecha))
        return y

    def _rotar_izquierda(self, z):
        y = z.derecha
        T2 = y.izquierda
        y.izquierda = z
        z.derecha = T2
        z.altura = 1 + max(self._altura(z.izquierda), self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), self._altura(y.derecha))
        return y

# -------------------- Disco LBA --------------------
class DISCOLBA:
    def __init__(self, platos=2, pistas=10, sectores=100, tamano_sector=64):
        self.platos = platos
        self.pistas = pistas
        self.sectores_por_pista = sectores
        self.total_sectores = platos * pistas * sectores
        self.tamano_sector = tamano_sector
        self.sectores = [None] * self.total_sectores
        self.mapa_ubicacion = {}

    def guardar_dato(self, dato_str, id_registro):
        bytes_restantes = len(dato_str.encode('utf-8'))
        bloques_usados = []

        while bytes_restantes > 0:
            espacio = min(bytes_restantes, self.tamano_sector)
            lba_index = self._buscar_sector_libre()

            if lba_index is None:
                self._liberar_bloques(bloques_usados)
                raise MemoryError("¡Disco lleno!")

            fragmento = dato_str[:espacio]
            self.sectores[lba_index] = fragmento
            bloques_usados.append(lba_index)
            dato_str = dato_str[espacio:]
            bytes_restantes -= espacio

        self.mapa_ubicacion[id_registro] = bloques_usados

    def _buscar_sector_libre(self):
        for i, contenido in enumerate(self.sectores):
            if contenido is None:
                return i
        return None

    def _liberar_bloques(self, bloques):
        for i in bloques:
            self.sectores[i] = None

    def recuperar_dato(self, id_registro):
        if id_registro not in self.mapa_ubicacion:
            return None
        return ''.join(self.sectores[i] for i in self.mapa_ubicacion[id_registro])

# -------------------- Base de datos --------------------
class BaseDeDatosFiltrado:
    def __init__(self):
        self.tablas = {}
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
            lbas = self.disco.mapa_ubicacion[id_registro]
            for col, valor in registro.items():
                tabla['indices'][col].insertar(valor, id_registro, lbas)
            tabla['registros'][id_registro] = registro

    def select(self, nombre_tabla, condiciones=None):
        if nombre_tabla not in self.tablas:
            return []

        resultados = []
        tabla = self.tablas[nombre_tabla]
        registros = tabla['registros']

        if not condiciones:
            for id_reg, reg in registros.items():
                resultados.append({
                    'registro': reg,
                    'id': id_reg,
                    'ubicacion_disco': self.disco.mapa_ubicacion[id_reg]
                })
            return resultados

        ids_validos = set()

        for i, (col, op, val) in enumerate(condiciones):
            if col not in tabla['indices']:
                continue

            # Solo permitir operador de igualdad
            if op != '=':
                messagebox.showerror("Error", "Solo se permite el operador de igualdad (=)")
                return []
                
            coincidencias = tabla['indices'][col].buscar(val)
            ids_actuales = {id_reg for id_reg, _ in coincidencias}

            if i == 0:
                ids_validos = ids_actuales
            else:
                ids_validos &= ids_actuales  # AND lógico entre condiciones

        for id_reg in ids_validos:
            reg = registros[id_reg]
            resultados.append({
                'registro': reg,
                'id': id_reg,
                'ubicacion_disco': self.disco.mapa_ubicacion[id_reg]
            })

        return resultados

def iniciar_interfaz():
    def ejecutar_consulta():
        condiciones = []
        col = combo_columna.get()
        op = combo_operador.get()
        val = entry_valor.get()

        if col and op and val:
            tipo = db.tablas['empleados']['tipos'][col]
            try:
                if tipo == int:
                    val = int(val)
                elif tipo == float:
                    val = float(val)
            except:
                messagebox.showerror("Error", "Tipo de dato inválido para la columna")
                return
            condiciones.append((col, op, val))

        nonlocal resultados_global
        resultados_global = db.select("empleados", condiciones if condiciones else None)

        listbox_resultados.delete(0, tk.END)
        for r in resultados_global:
            listbox_resultados.insert(tk.END, f"{r['registro']}")

    def mostrar_detalle():
        seleccion = listbox_resultados.curselection()
        if not seleccion:
            messagebox.showwarning("Aviso", "Selecciona un registro para ver los detalles de almacenamiento.")
            return
        idx = seleccion[0]
        id_registro = resultados_global[idx]['id']

        bloques = db.disco.mapa_ubicacion.get(id_registro, [])
        if not bloques:
            detalle = "No se encontraron bloques para este registro."
        else:
            detalle = f"Registro ID: {id_registro}\nUbicado en LBAs: {bloques}\n\nContenido en sectores:\n"
            for lba in bloques:
                contenido = db.disco.sectores[lba]
                plato = lba // (db.disco.pistas * db.disco.sectores_por_pista)
                resto = lba % (db.disco.pistas * db.disco.sectores_por_pista)
                pista = resto // db.disco.sectores_por_pista
                sector = resto % db.disco.sectores_por_pista
                detalle += f"LBA {lba} -> Plato {plato}, Pista {pista}, Sector {sector}: {contenido}\n"

        messagebox.showinfo(f"Detalles de {id_registro}", detalle)

    root = tk.Tk()
    root.title("Interfaz de Consulta de Empleados")

    tk.Label(root, text="Columna:").pack()
    combo_columna = ttk.Combobox(root, values=db.tablas['empleados']['columnas'], state='readonly')
    combo_columna.pack()

    tk.Label(root, text="Operador:").pack()
    # Solo mostrar el operador de igualdad
    combo_operador = ttk.Combobox(root, values=['='], state='readonly')
    combo_operador.pack()

    tk.Label(root, text="Valor:").pack()
    entry_valor = tk.Entry(root)
    entry_valor.pack()

    tk.Button(root, text="Ejecutar SELECT", command=ejecutar_consulta).pack(pady=5)

    listbox_resultados = tk.Listbox(root, width=100, height=10)
    listbox_resultados.pack(pady=10)

    tk.Button(root, text="Ver Detalles de Almacenamiento", command=mostrar_detalle).pack(pady=5)

    resultados_global = []
    root.mainloop()

# -------------------- MAIN PARA PRUEBAS --------------------
if __name__ == "__main__":
    db = BaseDeDatosFiltrado()

    empleados = [
        {"id": 1, "nombre": "Juan", "edad": 30, "departamento": "IT", "salario": 45000},
        {"id": 2, "nombre": "Ana", "edad": 25, "departamento": "Ventas", "salario": 38000},
        {"id": 3, "nombre": "Luis", "edad": 30, "departamento": "RRHH", "salario": 42000},
        {"id": 4, "nombre": "Marta", "edad": 35, "departamento": "IT", "salario": 50000},
        {"id": 5, "nombre": "Carlos", "edad": 28, "departamento": "Ventas", "salario": 36000},
        {"id": 6, "nombre": "Sofía", "edad": 32, "departamento": "Marketing", "salario": 48000},
        {"id": 7, "nombre": "Pedro", "edad": 40, "departamento": "IT", "salario": 65000},
        {"id": 8, "nombre": "Laura", "edad": 29, "departamento": "RRHH", "salario": 41000},
        {"id": 9, "nombre": "Diego", "edad": 27, "departamento": "Ventas", "salario": 37000},
        {"id": 10, "nombre": "Elena", "edad": 38, "departamento": "IT", "salario": 55000},
        {"id": 11, "nombre": "Miguel", "edad": 31, "departamento": "Marketing", "salario": 49000},
        {"id": 12, "nombre": "Isabel", "edad": 26, "departamento": "Ventas", "salario": 39000},
        {"id": 13, "nombre": "Javier", "edad": 33, "departamento": "IT", "salario": 52000},
        {"id": 14, "nombre": "Carmen", "edad": 40, "departamento": "RRHH", "salario": 47000},
        {"id": 15, "nombre": "Ricardo", "edad": 29, "departamento": "Marketing", "salario": 43000},
        {"id": 16, "nombre": "Patricia", "edad": 36, "departamento": "IT", "salario": 58000},
        {"id": 17, "nombre": "Fernando", "edad": 40, "departamento": "Ventas", "salario": 51000},
        {"id": 18, "nombre": "Lucía", "edad": 24, "departamento": "Marketing", "salario": 40000},
        {"id": 19, "nombre": "Roberto", "edad": 37, "departamento": "IT", "salario": 60000},
        {"id": 20, "nombre": "Beatriz", "edad": 20, "departamento": "RRHH", "salario": 44000},
        {"id": 21, "nombre": "Alberto", "edad": 31, "departamento": "Ventas", "salario": 46000},
        {"id": 22, "nombre": "Silvia", "edad": 40, "departamento": "Marketing", "salario": 52000},
        {"id": 23, "nombre": "Raúl", "edad": 39, "departamento": "IT", "salario": 62000},
        {"id": 24, "nombre": "Olga", "edad": 27, "departamento": "RRHH", "salario": 43000}
    ]

    db.cargar_datos("empleados", empleados)

    iniciar_interfaz()