#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo* ant;
    Nodo* sig;
    Nodo(int d) : dato(d), ant(nullptr), sig(nullptr) {}
};

class ListaDoble {
    Nodo* cabeza;
public:
    ListaDoble() : cabeza(nullptr) {}
    
    void insertarFinal(int valor) {
        Nodo* nuevo = new Nodo(valor);
        if (!cabeza) {
            cabeza = nuevo;
            return;
        }
        
        Nodo* actual = cabeza;
        while (actual->sig) {
            actual = actual->sig;
        }
        actual->sig = nuevo;
        nuevo->ant = actual;
    }
    
    void reorganizarPivote(int pivote) {
        if (!cabeza) return;
        
        Nodo* menor = cabeza;
        Nodo* actual = cabeza;
        
        while (actual) {
            if (actual->dato < pivote) {
                swap(actual->dato, menor->dato);
                menor = menor->sig;
            }
            actual = actual->sig;
        }
    }
    
    void mostrar() {
        Nodo* actual = cabeza;
        while (actual) {
            cout << actual->dato << " ";
            actual = actual->sig;
        }
        cout << endl;
    }
};

int main() {
    ListaDoble lista;
    int arr[] = {7, 3, 9, 2, 10, 1, 8};
    int n = sizeof(arr)/sizeof(arr[0]);
    int pivote = 5;
    
    for (int i = 0; i < n; i++) {
        lista.insertarFinal(arr[i]);
    }
    
    cout << "Lista original: ";
    lista.mostrar();
    
    lista.reorganizarPivote(pivote);
    
    cout << "Lista con pivote " << pivote << ": ";
    lista.mostrar();
    
    return 0;
}