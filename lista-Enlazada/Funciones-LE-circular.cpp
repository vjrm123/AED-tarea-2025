#include <iostream>

struct Node {
    int Value;
    Node* Next;
    Node(int Value, Node* Next = nullptr) : Value(Value), Next(Next) {}
};

struct LE_Cir {
    Node* Head = nullptr;

    void Add(int Value) {
        // Caso lista vacía
        if (Head == nullptr) {
            Head = new Node(Value);
            Head->Next = Head;
            return;
        }

        Node* current = Head;
        Node* prev = nullptr;
        bool inserted = false;

        // Buscar posición de inserción
        do {
            if (current->Value == Value) return; // Evitar duplicados
            if (current->Value > Value) {
                // Insertar aquí
                Node* newNode = new Node(Value, current);
                if (prev == nullptr) {
                    // Insertar al principio
                    Node* last = Head;
                    while (last->Next != Head) last = last->Next;
                    last->Next = newNode;
                    Head = newNode;
                } else {
                    prev->Next = newNode;
                }
                inserted = true;
                break;
            }
            prev = current;
            current = current->Next;
        } while (current != Head);

        // Insertar al final si no se insertó antes
        if (!inserted) {
            Node* newNode = new Node(Value, Head);
            prev->Next = newNode;
        }
    }

    void del(int Value) {
        if (Head == nullptr) return;

        Node* current = Head;
        Node* prev = nullptr;
        bool found = false;

        // Buscar nodo a eliminar
        do {
            if (current->Value == Value) {
                found = true;
                break;
            }
            prev = current;
            current = current->Next;
        } while (current != Head);

        if (!found) return;

        // Caso único nodo
        if (current->Next == current) {
            delete current;
            Head = nullptr;
            return;
        }

        // Eliminar nodo
        if (current == Head) {
            Node* last = Head;
            while (last->Next != Head) last = last->Next;
            Head = Head->Next;
            last->Next = Head;
        } else {
            prev->Next = current->Next;
        }

        delete current;
    }

    void Print() {
        if (!Head) {
            std::cout << "Lista vacía\n";
            return;
        }
        Node* Current = Head;
        do {
            std::cout << Current->Value << " ";
            Current = Current->Next;
        } while (Current != Head);
        std::cout << std::endl;
    }

    ~LE_Cir() {
        if (!Head) return;
        Node* Current = Head;
        do {
            Node* Temp = Current;
            Current = Current->Next;
            delete Temp;
        } while (Current != Head);
        Head = nullptr;
    }
};

int main() {
    LE_Cir lista;
    lista.Add(3);
    lista.Add(1);
    lista.Add(2);
    lista.Add(5);
    lista.Print(); // Debería imprimir: 1 2 3 5

    lista.del(3);
    lista.Print(); // Debería imprimir: 1 2 5

    return 0;
}

