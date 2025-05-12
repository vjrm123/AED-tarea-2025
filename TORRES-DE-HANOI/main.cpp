#include <iostream>
#include <stack>
using namespace std;

void mostrarTorres(const stack<int>& A, const stack<int>& B, const stack<int>& C) {
    stack<int> temp;

    cout << "Torre A: ";
    temp = A;
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;

    cout << "Torre B: ";
    temp = B;
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;

    cout << "Torre C: ";
    temp = C;
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl << "----------------" << endl;
}

void moverDisco(stack<int>& origen, stack<int>& destino, char nombreOrigen, char nombreDestino) {
    destino.push(origen.top());
    origen.pop();
    cout << "Mover disco " << destino.top() << " de " << nombreOrigen << " a " << nombreDestino << endl;
}

void hanoi(int n, stack<int>& A, stack<int>& C, stack<int>& B, char nombreA, char nombreC, char nombreB) {
    if (n == 1) {
        moverDisco(A, C, nombreA, nombreC);
        mostrarTorres(A, B, C);
        return;
    }

    hanoi(n - 1, A, B, C, nombreA, nombreB, nombreC);
    moverDisco(A, C, nombreA, nombreC);
    mostrarTorres(A, B, C);
    hanoi(n - 1, B, C, A, nombreB, nombreC, nombreA);
} 

int main() {
    stack<int> A, B, C;
    int discos = 8; 

    for (int i = discos; i >= 1; --i) {
        A.push(i);
    }

    cout << "Estado inicial:" << endl;
    mostrarTorres(A, B, C);

    hanoi(discos, A, C, B, 'A', 'C', 'B');

    return 0;
}