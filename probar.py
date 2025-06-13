import tkinter as tk
from tkinter import ttk, messagebox

# ------------------------- AVL -------------------------
class NodoAVL:
    def __init__(self, clave, valores):
        self.clave = clave
        self.valores = valores  # Lista de tuplas (id_registro, posicion_disco)
        self.izquierda = None
        self.derecha = None
        self.altura = 1

class AVLIndex:
    def __init__(self):
        self.raiz = None

    def insertar(self, clave, id_registro, posicion_disco):
        self.raiz = self._insertar(self.raiz, clave, id_registro, posicion_disco)

    def _insertar(self, nodo, clave, id_registro, posicion_disco):
        if not nodo:
            return NodoAVL(clave, [(id_registro, posicion_disco)])
        if clave < nodo.clave:
            nodo.izquierda = self._insertar(nodo.izquierda, clave, id_registro, posicion_disco)
        elif clave > nodo.clave:
            nodo.derecha = self._insertar(nodo.derecha, clave, id_registro, posicion_disco)
        else:
            nodo.valores.append((id_registro, posicion_disco))
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

    def _altura(self, nodo):
        return nodo.altura if nodo else 0

    def _balance(self, nodo):
        return self._altura(nodo.izquierda) - self._altura(nodo.derecha)

    def _rotar_izquierda(self, z):
        y = z.derecha
        T2 = y.izquierda
        y.izquierda = z
        z.derecha = T2
        z.altura = 1 + max(self._altura(z.izquierda), self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), self._altura(y.derecha))
        return y

    def _rotar_derecha(self, z):
        y = z.izquierda
        T3 = y.derecha
        y.derecha = z
        z.izquierda = T3
        z.altura = 1 + max(self._altura(z.izquierda), self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), self._altura(y.derecha))
        return y

# ------------------------- DISCO -------------------------
class DiscoDuroMejorado:
    def __init__(self):
        self.almacenamiento = {}

    def guardar(self, id_registro, datos):
        self.almacenamiento[id_registro] = datos
        return id_registro

    def recuperar(self, id_registro):
        return self.almacenamiento.get(id_registro)

# ------------------------- BASE DE DATOS -------------------------
class BaseDeDatosFiltrado:
    def __init__(self):
        self.tablas = {}
        self.disco = DiscoDuroMejorado()

    def cargar_datos(self, nombre_tabla, datos):
        if nombre_tabla not in self.tablas:
            columnas = list(datos[0].keys()) if datos else []
            self.tablas[nombre_tabla] = {
                'columnas': columnas,
                'indices': {col: AVLIndex() for col in columnas},
                'registros': {}
            }

        tabla = self.tablas[nombre_tabla]
        for registro in datos:
            id_registro = f"{nombre_tabla}_{len(tabla['registros']) + 1}"
            posicion = self.disco.guardar(id_registro, registro)
            for col, valor in registro.items():
                tabla['indices'][col].insertar(valor, id_registro, posicion)
            tabla['registros'][id_registro] = registro

    def select(self, nombre_tabla, condiciones=None):
        if nombre_tabla not in self.tablas:
            return []

        if not condiciones:
            return list(self.tablas[nombre_tabla]['registros'].values())

        # Solo permite búsquedas exactas con "="
        resultados = []
        for cond in condiciones:
            columna, operador, valor = cond
            if operador != "=":
                continue  # Ignora operadores que no sean igualdad
            if columna not in self.tablas[nombre_tabla]['indices']:
                continue
            
            # Usa el índice AVL para búsqueda exacta
            ids_posiciones = self.tablas[nombre_tabla]['indices'][columna].buscar(valor)
            for id_reg, pos in ids_posiciones:
                registro = self.disco.recuperar(id_reg)
                if registro:
                    resultados.append(registro)
        
        return resultados

# ------------------------- INTERFAZ -------------------------
class InterfazFiltrado:
    def __init__(self, root):
        self.root = root
        self.root.title("Sistema de Filtrado con AVL (Solo =)")
        self.db = BaseDeDatosFiltrado()
        self._cargar_datos_ejemplo()
        self.crear_widgets()

    def _cargar_datos_ejemplo(self):
        empleados = [
            {"id": 1, "nombre": "Juan Pérez", "edad": 32, "departamento": "Ventas", "salario": 45000},
            {"id": 2, "nombre": "María García", "edad": 28, "departamento": "IT", "salario": 55000},
            {"id": 3, "nombre": "Carlos López", "edad": 45, "departamento": "Ventas", "salario": 48000},
            {"id": 4, "nombre": "Ana Martínez", "edad": 30, "departamento": "RH", "salario": 42000},
            {"id": 5, "nombre": "Pedro Sánchez", "edad": 35, "departamento": "IT", "salario": 60000}
        ]
        self.db.cargar_datos("empleados", empleados)

    def crear_widgets(self):
        frame_tabla = ttk.LabelFrame(self.root, text="Tabla")
        frame_tabla.pack(fill=tk.X, padx=10, pady=5)
        self.combo_tablas = ttk.Combobox(frame_tabla, values=list(self.db.tablas.keys()), state="readonly")
        self.combo_tablas.pack(fill=tk.X, padx=5, pady=5)
        self.combo_tablas.bind("<<ComboboxSelected>>", self.actualizar_columnas)
        if self.db.tablas:
            self.combo_tablas.set(list(self.db.tablas.keys())[0])

        frame_filtros = ttk.LabelFrame(self.root, text="Filtrado (Solo =)")
        frame_filtros.pack(fill=tk.X, padx=10, pady=5)
        self.frame_condiciones = ttk.Frame(frame_filtros)
        self.frame_condiciones.pack(fill=tk.X)
        ttk.Button(frame_filtros, text="Agregar condición", command=self.agregar_condicion).pack(pady=5)

        frame_resultados = ttk.LabelFrame(self.root, text="Resultados")
        frame_resultados.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        self.tree = ttk.Treeview(frame_resultados)
        self.tree.pack(fill=tk.BOTH, expand=True)
        ttk.Scrollbar(frame_resultados, orient="vertical", command=self.tree.yview).pack(side=tk.RIGHT, fill=tk.Y)
        self.tree.configure(yscrollcommand=lambda f: None)
        ttk.Button(self.root, text="Ejecutar Consulta", command=self.ejecutar_consulta).pack(pady=10)
        self.agregar_condicion()

    def agregar_condicion(self):
        tabla = self.combo_tablas.get()
        columnas = self.db.tablas[tabla]['columnas'] if tabla in self.db.tablas else []
        frame = ttk.Frame(self.frame_condiciones)
        frame.pack(fill=tk.X, pady=2)
        combo_col = ttk.Combobox(frame, values=columnas, state='readonly', width=15)
        combo_col.pack(side=tk.LEFT, padx=2)
        combo_op = ttk.Combobox(frame, values=["="], state='readonly', width=5)  # Solo permite "="
        combo_op.pack(side=tk.LEFT, padx=2)
        combo_op.set("=")
        entry_valor = ttk.Entry(frame, width=20)
        entry_valor.pack(side=tk.LEFT, padx=2)
        ttk.Button(frame, text="X", width=3, command=lambda f=frame: f.destroy()).pack(side=tk.LEFT, padx=2)

    def actualizar_columnas(self, event):
        for widget in self.frame_condiciones.winfo_children():
            widget.destroy()
        self.agregar_condicion()

    def ejecutar_consulta(self):
        tabla = self.combo_tablas.get()
        if not tabla:
            messagebox.showerror("Error", "Selecciona una tabla")
            return
        condiciones = []
        for frame in self.frame_condiciones.winfo_children():
            widgets = frame.winfo_children()
            if len(widgets) >= 3:
                columna = widgets[0].get()
                operador = widgets[1].get()
                valor = widgets[2].get()
                if not columna:
                    continue
                try:
                    # Intenta convertir a número si es posible
                    valor = int(valor) if valor.isdigit() else valor
                except ValueError:
                    pass
                condiciones.append((columna, operador, valor))
        resultados = self.db.select(tabla, condiciones)
        self.mostrar_resultados(tabla, resultados)

    def mostrar_resultados(self, tabla, resultados):
        self.tree.delete(*self.tree.get_children())
        if not resultados:
            messagebox.showinfo("Sin resultados", "No se encontraron registros")
            return
        columnas = self.db.tablas[tabla]['columnas']
        self.tree['columns'] = columnas
        self.tree.heading('#0', text='ID')
        for col in columnas:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=100)
        for registro in resultados:
            self.tree.insert('', 'end', text=str(registro.get('id', '')), values=[registro.get(col, '') for col in columnas])

if __name__ == "__main__":
    root = tk.Tk()
    app = InterfazFiltrado(root)
    root.mainloop()