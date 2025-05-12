#include <iostream>
#include <queue>
using namespace std;

struct node {
    int valor_mostrado; // suma de ancestros
    int guia;           // valor guía original usado para decidir hacia dónde ir
    node* nod[2];

    node(int v_mostrado, int v_guia) : valor_mostrado(v_mostrado), guia(v_guia) {
        nod[0] = nod[1] = nullptr;
    }
};

class cbintree {
    node* root;

public:
    cbintree() : root(nullptr) {}

    bool insert(int guia) {
        if (!root) {
            root = new node(guia, guia); 
            return true;
        }

        node* actual = root;
        int suma = 0;

        while (true) {
            suma += actual->valor_mostrado;

            int dir = guia > actual->guia; 
            if (!actual->nod[dir]) {
                actual->nod[dir] = new node(suma, guia);
                return true;
            }

            actual = actual->nod[dir];
        }
    }


    void printPorNiveles() {
        if (!root) return;
        queue<node*> q;
        q.push(root);

        while (!q.empty()) {
            int nivel = q.size();
            while (nivel--) {
                node* actual = q.front(); q.pop();
                cout << "(" << actual->valor_mostrado << "\t";
                if (actual->nod[0]) q.push(actual->nod[0]);
                if (actual->nod[1]) q.push(actual->nod[1]);
            }
            cout << endl;
        }
    }
};

int main() {
    cbintree arbol;
    arbol.insert(10);
    arbol.insert(5);
    arbol.insert(15);
    arbol.insert(3);
    arbol.insert(7);
    arbol.insert(12);
    arbol.insert(20);

    cout << "Árbol con suma de ancestros (por niveles):\n";
    arbol.printPorNiveles();

    return 0;
}
