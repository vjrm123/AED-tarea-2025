import tkinter as tk
from tkinter import ttk, messagebox
from disco_duro import DISCODURO

class InterfazDiscoDuro:
    def __init__(self, root):
        self.root = root
        self.root.title("Gestor de Disco Duro")
        self.disco = DISCODURO(tamano_sector=32)
        
        # Controles superiores (Ingreso de datos)
        self.frame_ingreso = ttk.LabelFrame(root, text="Ingresar Nuevo Dato")
        self.frame_ingreso.pack(pady=10, padx=10, fill=tk.X)
        
        ttk.Label(self.frame_ingreso, text="ID:").grid(row=0, column=0, padx=5)
        self.entry_id = ttk.Entry(self.frame_ingreso)
        self.entry_id.grid(row=0, column=1, padx=5)
        
        ttk.Label(self.frame_ingreso, text="Dato:").grid(row=1, column=0, padx=5)
        self.entry_dato = ttk.Entry(self.frame_ingreso, width=40)
        self.entry_dato.grid(row=1, column=1, padx=5)
        
        self.btn_guardar = ttk.Button(
            self.frame_ingreso, 
            text="Guardar Dato", 
            command=self.guardar_dato
        )
        self.btn_guardar.grid(row=2, columnspan=2, pady=5)

        # Controles de búsqueda/visualización
        self.frame_busqueda = ttk.LabelFrame(root, text="Buscar Dato")
        self.frame_busqueda.pack(pady=10, padx=10, fill=tk.X)
        
        ttk.Label(self.frame_busqueda, text="ID a buscar:").pack(side=tk.LEFT)
        self.entry_buscar_id = ttk.Entry(self.frame_busqueda, width=15)
        self.entry_buscar_id.pack(side=tk.LEFT, padx=5)
        
        self.btn_buscar = ttk.Button(
            self.frame_busqueda, 
            text="Buscar", 
            command=self.mostrar_resumen
        )
        self.btn_buscar.pack(side=tk.LEFT)
        
        # Área de resultados
        self.frame_resultados = ttk.Frame(root)
        self.frame_resultados.pack(pady=10, padx=10, fill=tk.BOTH, expand=True)
        
        self.label_resumen = ttk.Label(
            self.frame_resultados, 
            text="Ingrese un ID y pulse 'Buscar'",
            wraplength=500
        )
        self.label_resumen.pack(anchor=tk.W)
        
        self.btn_detalles = ttk.Button(
            self.frame_resultados,
            text="Ver detalles técnicos",
            command=self.mostrar_detalles,
            state=tk.DISABLED
        )
        
        self.text_detalles = tk.Text(
            self.frame_resultados,
            height=10,
            width=60,
            wrap=tk.WORD,
            state=tk.DISABLED
        )
        self.scroll = ttk.Scrollbar(
            self.frame_resultados,
            command=self.text_detalles.yview
        )
        self.text_detalles.configure(yscrollcommand=self.scroll.set)
    
    def guardar_dato(self):
        id_registro = self.entry_id.get()
        dato = self.entry_dato.get()
        
        if not id_registro or not dato:
            messagebox.showerror("Error", "Debe ingresar un ID y un dato")
            return
        
        try:
            self.disco.guardar_dato(dato, id_registro)
            messagebox.showinfo("Éxito", f"Dato '{id_registro}' guardado correctamente")
            self.entry_id.delete(0, tk.END)
            self.entry_dato.delete(0, tk.END)
        except MemoryError as e:
            messagebox.showerror("Error", str(e))
    
    def mostrar_resumen(self):
        id_registro = self.entry_buscar_id.get()
        resumen = self.disco.resumen_almacenamiento(id_registro)
        
        if resumen:
            self.label_resumen.config(text=resumen)
            self.btn_detalles.pack(pady=5)
            self.btn_detalles.config(state=tk.NORMAL)
            self.text_detalles.pack_forget()
        else:
            self.label_resumen.config(text=f"⚠ ID '{id_registro}' no encontrado")
            self.btn_detalles.pack_forget()
            self.text_detalles.pack_forget()
    
    def mostrar_detalles(self):
        id_registro = self.entry_buscar_id.get()
        detalle = self.disco.detalle_almacenamiento(id_registro)
        
        self.text_detalles.config(state=tk.NORMAL)
        self.text_detalles.delete(1.0, tk.END)
        self.text_detalles.insert(tk.END, detalle)
        self.text_detalles.config(state=tk.DISABLED)
        
        self.text_detalles.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.scroll.pack(side=tk.RIGHT, fill=tk.Y)

if __name__ == "__main__":
    root = tk.Tk()
    app = InterfazDiscoDuro(root)
    root.mainloop()