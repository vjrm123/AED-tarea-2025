#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Nodo {
    int valor;
    Nodo* hijos[2];

    Nodo(int val) : valor(val) {
        hijos[0] = hijos[1] = nullptr;
    }
};

class ArbolVisual {
private:
    Nodo* raiz;

    void limpiar(Nodo* nodo) {
        if (!nodo) return;
        limpiar(nodo->hijos[0]);
        limpiar(nodo->hijos[1]);
        delete nodo;
    }

public:
    ArbolVisual() : raiz(nullptr) {}

    ~ArbolVisual() {
        limpiar(raiz);
    }

    void construirDesdeHojas(const vector<int>& hojas) {
        limpiar(raiz);
        if (hojas.empty()) return;

        queue<Nodo*> nivelActual;
        for (int hoja : hojas) {
            nivelActual.push(new Nodo(hoja));
        }

        while (nivelActual.size() > 1) {
            queue<Nodo*> siguienteNivel;

            while (!nivelActual.empty()) {
                Nodo* izquierdo = nivelActual.front();
                nivelActual.pop();

                Nodo* derecho = nullptr;
                if (!nivelActual.empty()) {
                    derecho = nivelActual.front();
                    nivelActual.pop();
                }

                Nodo* padre = new Nodo(izquierdo->valor + (derecho ? derecho->valor : 0));
                padre->hijos[0] = izquierdo;
                padre->hijos[1] = derecho;
                siguienteNivel.push(padre);
            }

            nivelActual = siguienteNivel;
        }

        raiz = nivelActual.front();
    }

    void imprimirPorNiveles() {
        if (!raiz) return;

        queue<Nodo*> q;
        q.push(raiz);

        while (!q.empty()) {
            int tam = q.size();
            while (tam--) {
                Nodo* actual = q.front(); q.pop();
                cout << actual->valor << " ";
                if (actual->hijos[0]) q.push(actual->hijos[0]);
                if (actual->hijos[1]) q.push(actual->hijos[1]);
            }
            cout << endl;
        }
    }
};

int main() {
    ArbolVisual arbol;
    vector<int> hojas = { 3, 5, 2, 7, 1, 4, 6, 5 };

    arbol.construirDesdeHojas(hojas);
    arbol.imprimirPorNiveles();

    return 0;
}
