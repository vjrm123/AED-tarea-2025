import tkinter as tk
from tkinter import ttk, messagebox, filedialog, scrolledtext
import csv

# ------------------------- ESTRUCTURAS DE DATOS -------------------------
class NodoAVL:
    def __init__(self, clave, valor):
        self.clave = clave
        self.valor = valor  # Lista de IDs que comparten este valor
        self.izquierda = None
        self.derecha = None
        self.altura = 1

class AVL:
    def __init__(self):
        self.raiz = None
    
    def insertar(self, clave, id_registro):
        if self.raiz is None:
            self.raiz = NodoAVL(clave, [id_registro])
        else:
            self.raiz = self._insertar(self.raiz, clave, id_registro)
    
    def _insertar(self, nodo, clave, id_registro):
        if not nodo:
            return NodoAVL(clave, [id_registro])
        
        if clave < nodo.clave:
            nodo.izquierda = self._insertar(nodo.izquierda, clave, id_registro)
        elif clave > nodo.clave:
            nodo.derecha = self._insertar(nodo.derecha, clave, id_registro)
        else:
            nodo.valor.append(id_registro)
            return nodo
        
        nodo.altura = 1 + max(self._altura(nodo.izquierda), 
                            self._altura(nodo.derecha))
        
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
            return nodo.valor
        elif clave < nodo.clave:
            return self._buscar(nodo.izquierda, clave)
        else:
            return self._buscar(nodo.derecha, clave)
    
    def _altura(self, nodo):
        if not nodo:
            return 0
        return nodo.altura
    
    def _balance(self, nodo):
        if not nodo:
            return 0
        return self._altura(nodo.izquierda) - self._altura(nodo.derecha)
    
    def _rotar_derecha(self, z):
        y = z.izquierda
        T3 = y.derecha
        
        y.derecha = z
        z.izquierda = T3
        
        z.altura = 1 + max(self._altura(z.izquierda), 
                        self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), 
                        self._altura(y.derecha))
        
        return y
    
    def _rotar_izquierda(self, z):
        y = z.derecha
        T2 = y.izquierda
        
        y.izquierda = z
        z.derecha = T2
        
        z.altura = 1 + max(self._altura(z.izquierda), 
                        self._altura(z.derecha))
        y.altura = 1 + max(self._altura(y.izquierda), 
                        self._altura(y.derecha))
        
        return y

class DISCODURO:
    def __init__(self, platos=2, pistas=10, sectores=100, tamano_sector=64):
        self.platos = [[[None for _ in range(sectores)] for _ in range(pistas)] for _ in range(platos)]
        self.mapa_ubicacion = {}  # {id: [(plato, pista, sector)]}
        self.tamano_sector = tamano_sector

    def guardar_dato(self, dato, id_registro):
        dato_str = str(dato)
        bytes_restantes = len(dato_str.encode('utf-8'))
        bloques_usados = []
        
        while bytes_restantes > 0:
            espacio = min(bytes_restantes, self.tamano_sector)
            sector_libre = self._buscar_sector_libre()
            
            if not sector_libre:
                self._liberar_bloques(bloques_usados)
                raise MemoryError("¡Disco lleno! No se pudo guardar el dato completo.")
            
            plato, pista, sector = sector_libre
            fragmento = dato_str[:espacio]
            self.platos[plato][pista][sector] = fragmento
            bloques_usados.append((plato, pista, sector))
            dato_str = dato_str[espacio:]
            bytes_restantes -= espacio
        
        self.mapa_ubicacion[id_registro] = bloques_usados
        return True

    def _buscar_sector_libre(self):
        for plato in range(len(self.platos)):
            for pista in range(len(self.platos[plato])):
                for sector in range(len(self.platos[plato][pista])):
                    if self.platos[plato][pista][sector] is None:
                        return (plato, pista, sector)
        return None

    def _liberar_bloques(self, bloques):
        for (plato, pista, sector) in bloques:
            self.platos[plato][pista][sector] = None

    def recuperar_dato(self, id_registro):
        if id_registro not in self.mapa_ubicacion:
            return None
        
        dato = ""
        for (plato, pista, sector) in self.mapa_ubicacion[id_registro]:
            dato += self.platos[plato][pista][sector]
        return eval(dato)  # Convertir de string a objeto original

    def detalle_almacenamiento(self, id_registro):
        if id_registro not in self.mapa_ubicacion:
            return None
        
        detalle = f"🔍 Detalles de almacenamiento - ID: {id_registro}\n\n"
        bloques = self.mapa_ubicacion[id_registro]
        dato_completo = self.recuperar_dato(id_registro)
        
        for i, (plato, pista, sector) in enumerate(bloques, 1):
            fragmento = self.platos[plato][pista][sector]
            inicio_byte = (i-1) * self.tamano_sector
            fin_byte = inicio_byte + len(fragmento.encode('utf-8')) - 1
            
            detalle += (
                f"🔹 Bloque {i}: Plato {plato}, Pista {pista}, Sector {sector}\n"
                f"   ▶ Bytes: {inicio_byte}-{fin_byte}\n"
                f"   ▶ Fragmento: '{fragmento}'\n"
                f"   ▶ Tamaño: {len(fragmento.encode('utf-8'))} bytes\n"
                f"   {'─'*40}\n"
            )
        
        detalle += f"\n📝 Dato completo reunido:\n{dato_completo}"
        return detalle

class BaseDeDatos:
    def __init__(self):
        self.tablas = {}
        self.disco = DISCODURO()
    
    def crear_tabla(self, nombre, columnas_indexadas):
        if nombre in self.tablas:
            raise ValueError(f"Tabla {nombre} ya existe")
        
        self.tablas[nombre] = {
            'indices': {col: AVL() for col in columnas_indexadas},
            'columnas': columnas_indexadas,
            'ultimo_id': 0
        }
    
    def insertar(self, tabla, valores):
        if tabla not in self.tablas:
            raise ValueError(f"Tabla {tabla} no existe")
        
        nuevo_id = self.tablas[tabla]['ultimo_id'] + 1
        self.tablas[tabla]['ultimo_id'] = nuevo_id
        
        registro = dict(zip(self.tablas[tabla]['columnas'], valores))
        
        # Guardar en disco duro
        self.disco.guardar_dato(registro, f"{tabla}_{nuevo_id}")
        
        # Indexar en AVLs
        for col, avl in self.tablas[tabla]['indices'].items():
            valor_col = registro[col]
            avl.insertar(valor_col, nuevo_id)
        
        return nuevo_id
    
    def buscar(self, tabla, columna, valor):
        if tabla not in self.tablas:
            raise ValueError(f"Tabla {tabla} no existe")
        if columna not in self.tablas[tabla]['indices']:
            raise ValueError(f"Columna {columna} no está indexada")
        
        ids = self.tablas[tabla]['indices'][columna].buscar(valor)
        return [self.disco.recuperar_dato(f"{tabla}_{id}") for id in ids]
    
    def obtener_ubicacion(self, tabla, id_registro):
        return self.disco.detalle_almacenamiento(f"{tabla}_{id_registro}")

# ------------------------- INTERFAZ GRÁFICA -------------------------
class InterfazGrafica:
    def __init__(self, root):
        self.root = root
        self.root.title("Base de Datos AVL con Disco Duro")
        self.db = BaseDeDatos()
        
        self.crear_widgets()
    
    def crear_widgets(self):
        # Panel de configuración
        frame_config = ttk.LabelFrame(self.root, text="Configuración")
        frame_config.pack(padx=10, pady=5, fill=tk.X)
        
        # Panel de creación de tablas
        frame_tablas = ttk.LabelFrame(self.root, text="Gestión de Tablas")
        frame_tablas.pack(padx=10, pady=5, fill=tk.X)
        
        ttk.Label(frame_tablas, text="Nombre Tabla:").grid(row=0, column=0, sticky=tk.W)
        self.entry_nombre_tabla = ttk.Entry(frame_tablas)
        self.entry_nombre_tabla.grid(row=0, column=1)
        
        ttk.Label(frame_tablas, text="Columnas (separadas por coma):").grid(row=1, column=0, sticky=tk.W)
        self.entry_columnas = ttk.Entry(frame_tablas, width=40)
        self.entry_columnas.grid(row=1, column=1)
        
        ttk.Button(frame_tablas, text="Crear Tabla", 
                  command=self.crear_tabla).grid(row=2, columnspan=2, pady=5)
        
        # Panel de inserción manual
        frame_insertar = ttk.LabelFrame(self.root, text="Inserción Manual")
        frame_insertar.pack(padx=10, pady=5, fill=tk.X)
        
        ttk.Label(frame_insertar, text="Datos (valores separados por coma):").grid(row=0, column=0, sticky=tk.W)
        self.entry_datos = ttk.Entry(frame_insertar, width=40)
        self.entry_datos.grid(row=0, column=1)
        
        ttk.Button(frame_insertar, text="Insertar Datos", 
                  command=self.insertar_manual).grid(row=1, columnspan=2, pady=5)
        
        # Panel de carga CSV
        frame_csv = ttk.LabelFrame(self.root, text="Carga desde CSV")
        frame_csv.pack(padx=10, pady=5, fill=tk.X)
        
        ttk.Button(frame_csv, text="Cargar CSV", 
                  command=self.cargar_csv).pack(pady=5)
        
        # Panel de búsqueda
        frame_busqueda = ttk.LabelFrame(self.root, text="Búsqueda")
        frame_busqueda.pack(padx=10, pady=5, fill=tk.BOTH, expand=True)
        
        ttk.Label(frame_busqueda, text="Tabla:").grid(row=0, column=0, sticky=tk.W)
        self.combo_tablas = ttk.Combobox(frame_busqueda, state='readonly')
        self.combo_tablas.grid(row=0, column=1, sticky=tk.EW)
        self.combo_tablas.bind('<<ComboboxSelected>>', self.actualizar_columnas)
        
        ttk.Label(frame_busqueda, text="Columna:").grid(row=1, column=0, sticky=tk.W)
        self.combo_columnas = ttk.Combobox(frame_busqueda, state='readonly')
        self.combo_columnas.grid(row=1, column=1, sticky=tk.EW)
        
        ttk.Label(frame_busqueda, text="Valor:").grid(row=2, column=0, sticky=tk.W)
        self.entry_valor = ttk.Entry(frame_busqueda)
        self.entry_valor.grid(row=2, column=1, sticky=tk.EW)
        
        ttk.Button(frame_busqueda, text="Buscar", 
                  command=self.buscar_datos).grid(row=3, columnspan=2, pady=5)
        
        # Resultados
        self.tree = ttk.Treeview(frame_busqueda)
        self.tree.grid(row=4, columnspan=2, sticky='nsew', pady=5)
        self.tree.bind('<<TreeviewSelect>>', self.mostrar_detalle_disco)
        
        # Detalle de almacenamiento en disco
        self.texto_detalle = scrolledtext.ScrolledText(frame_busqueda, height=8)
        self.texto_detalle.grid(row=5, columnspan=2, sticky='nsew', pady=5)
        
        scrollbar = ttk.Scrollbar(frame_busqueda, orient="vertical", command=self.tree.yview)
        scrollbar.grid(row=4, column=2, sticky='ns')
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        frame_busqueda.grid_rowconfigure(4, weight=1)
        frame_busqueda.grid_columnconfigure(1, weight=1)
    
    def crear_tabla(self):
        nombre = self.entry_nombre_tabla.get()
        columnas = [c.strip() for c in self.entry_columnas.get().split(',')]
        
        if not nombre or not columnas:
            messagebox.showerror("Error", "Nombre y columnas son requeridos")
            return
        
        try:
            self.db.crear_tabla(nombre, columnas)
            self.combo_tablas['values'] = list(self.db.tablas.keys())
            messagebox.showinfo("Éxito", f"Tabla {nombre} creada")
        except ValueError as e:
            messagebox.showerror("Error", str(e))
    
    def insertar_manual(self):
        tabla = self.combo_tablas.get()
        if not tabla:
            messagebox.showerror("Error", "Selecciona una tabla primero")
            return
        
        datos = [d.strip() for d in self.entry_datos.get().split(',')]
        if len(datos) != len(self.db.tablas[tabla]['columnas']):
            messagebox.showerror("Error", f"Número incorrecto de valores. Esperados: {len(self.db.tablas[tabla])['columnas']}")
            return
        
        try:
            id_nuevo = self.db.insertar(tabla, datos)
            messagebox.showinfo("Éxito", f"Datos insertados con ID: {id_nuevo}")
            self.entry_datos.delete(0, tk.END)
        except Exception as e:
            messagebox.showerror("Error", str(e))
    
    def cargar_csv(self):
        archivo = filedialog.askopenfilename(filetypes=[("CSV files", "*.csv")])
        if not archivo:
            return
        
        tabla = self.combo_tablas.get()
        if not tabla:
            messagebox.showerror("Error", "Selecciona una tabla primero")
            return
        
        try:
            with open(archivo, newline='', encoding='utf-8') as csvfile:
                lector = csv.reader(csvfile)
                contador = 0
                for fila in lector:
                    if len(fila) != len(self.db.tablas[tabla]['columnas']):
                        continue
                    self.db.insertar(tabla, fila)
                    contador += 1
            messagebox.showinfo("Éxito", f"{contador} registros cargados correctamente")
        except Exception as e:
            messagebox.showerror("Error", f"Error al cargar CSV: {str(e)}")
    
    def actualizar_columnas(self, event):
        tabla = self.combo_tablas.get()
        if tabla in self.db.tablas:
            self.combo_columnas['values'] = self.db.tablas[tabla]['columnas']
    
    def buscar_datos(self):
        tabla = self.combo_tablas.get()
        columna = self.combo_columnas.get()
        valor = self.entry_valor.get()
        
        if not all([tabla, columna, valor]):
            messagebox.showerror("Error", "Todos los campos son requeridos")
            return
        
        try:
            # Convertir valor numérico si es posible
            try:
                valor = int(valor)
            except ValueError:
                try:
                    valor = float(valor)
                except ValueError:
                    pass
            
            resultados = self.db.buscar(tabla, columna, valor)
            
            # Limpiar treeview
            for i in self.tree.get_children():
                self.tree.delete(i)
            
            # Configurar columnas
            if resultados:
                columnas = list(resultados[0].keys())
                self.tree['columns'] = columnas
                self.tree.heading('#0', text='ID')
                for col in columnas:
                    self.tree.heading(col, text=col)
                    self.tree.column(col, width=100)
                
                # Mostrar resultados
                for registro in resultados:
                    id_reg = registro.get('id', 'N/A')
                    self.tree.insert('', 'end', text=id_reg, 
                                   values=[registro[col] for col in columnas])
            
            if not resultados:
                messagebox.showinfo("Info", "No se encontraron resultados")
                self.texto_detalle.delete('1.0', tk.END)
        except Exception as e:
            messagebox.showerror("Error", str(e))
    
    def mostrar_detalle_disco(self, event):
        item = self.tree.focus()
        if not item:
            return
        
        id_registro = self.tree.item(item, 'text')
        tabla = self.combo_tablas.get()
        
        detalle = self.db.obtener_ubicacion(tabla, id_registro)
        self.texto_detalle.delete('1.0', tk.END)
        if detalle:
            self.texto_detalle.insert(tk.END, detalle)

if __name__ == "__main__":
    root = tk.Tk()
    app = InterfazGrafica(root)
    root.mainloop()