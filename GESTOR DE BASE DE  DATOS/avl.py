class NodoAVL:
    def __init__(self, clave, valor):
        self.clave = clave    
        self.valor = valor    
        self.izquierda = None
        self.derecha = None
        self.altura = 1       

class AVL:
    def __init__(self):
        self.raiz = None
    
    def insertar(self, clave, valor):
        self.raiz, insertado = self._insertar(self.raiz, clave, valor)
        if not insertado:
            raise ValueError(f"El ID {clave} ya existe")

    
    def _insertar(self, nodo, clave, valor):
        if not nodo:
            return NodoAVL(clave, valor), True

        if clave == nodo.clave:
            return nodo, False  

        if clave < nodo.clave:
            nodo.izquierda, insertado = self._insertar(nodo.izquierda, clave, valor)
        else:
            nodo.derecha, insertado = self._insertar(nodo.derecha, clave, valor)

        if not insertado:
            return nodo, False

        nodo.altura = 1 + max(self._altura(nodo.izquierda), self._altura(nodo.derecha))
        balance = self._balance(nodo)

        if balance > 1 and clave < nodo.izquierda.clave:
            return self._rotar_derecha(nodo), True
        if balance < -1 and clave > nodo.derecha.clave:
            return self._rotar_izquierda(nodo), True
        if balance > 1 and clave > nodo.izquierda.clave:
            nodo.izquierda = self._rotar_izquierda(nodo.izquierda)
            return self._rotar_derecha(nodo), True
        if balance < -1 and clave < nodo.derecha.clave:
            nodo.derecha = self._rotar_derecha(nodo.derecha)
            return self._rotar_izquierda(nodo), True

        return nodo, True

    
    def buscar(self, clave):
        return self._buscar(self.raiz, clave)
    
    def _buscar(self, nodo, clave):
        if not nodo:
            return None
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
    
    def in_order(self):
        return self._in_order(self.raiz)
    
    def _in_order(self, nodo):
        if not nodo:
            return []
        return self._in_order(nodo.izquierda) + [(nodo.clave, nodo.valor)] + self._in_order(nodo.derecha)
    

avl = AVL()

avl.insertar("003", ["Ana", 28])
avl.insertar("001", ["Juan", 30])
avl.insertar("002", ["Maria", 25])
avl.insertar("005", ["Carlos", 40])
avl.insertar("004", ["Luisa", 35])
print("Buscar ID 001:", avl.buscar("001"))

print("Buscar ID 006:", avl.buscar("002"))  

print("Recorrido in-order:", avl.in_order())