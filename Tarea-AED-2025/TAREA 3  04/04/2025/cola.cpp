#include <iostream>

template<class T>
struct Node {
    T elements[5];
    int ini = 0, fin = 0;
    Node* Next = NULL; 
};

template<class T>
struct Cola {
    Node<T>* Head = NULL;
    Node<T>* Tail = NULL;

    void Push(T Value);
    void Pop(T& value);
    void Print();
    void Print1();

    ~Cola();
};

template<class T>
void Cola<T>::Push(T value) {
    if (!Tail) Tail = Head = new Node<T>;
    if (Tail->fin == 5) { Tail = Tail->Next = new Node<T>; }
    Tail->elements[Tail->fin++] = value;
}

template<class T>
void Cola<T>::Pop(T& value) {
    if (!Head || Head->ini >= Head->fin) { std::cout << "Esta vacia!!\n"; return; }
    value = Head->elements[Head->ini++];
    if (Head->ini == Head->fin) {
        Node<T>* Temp = Head;
        Head = Head->Next;
        delete Temp;
        if (!Head) Tail = NULL;
    }
}

template<class T>
void Cola<T>::Print() {
    std::cout << "[ ";
    for (Node<T>* n = Head; n; n = n->Next) {
        std::cout << (n == Head ? "[" : "-> [");
        for (int i = n->ini, c = 0; i < n->fin; ++i) {
            std::cout << (c++ ? " " : "") << n->elements[i];
        }
        std::cout << "]";
    }
    std::cout << " ]\n";
}

template<class T>
void Cola<T>::Print1() {
    std::cout << "[";
    bool first = true; 
    for (Node<T>* n = Head; n; n = n->Next) {
        for (int i = n->ini; i < n->fin; ++i) {
            if (!first) std::cout << " ";
            std::cout << n->elements[i];
            first = false;
        }
    }
    std::cout << "]\n\n";
}

template<class T>
Cola<T>::~Cola() {
    T valor;
    while (Head) { 
        Pop(valor);
    }
}

int main() {
    Cola<int> c; int Value;
    c.Push(1); c.Push(2); c.Push(3); c.Push(4); c.Push(5);
    c.Push(6); c.Push(7); c.Push(8); c.Push(9); c.Push(10); c.Push(11);

    c.Print(); 

    c.Pop(Value); c.Pop(Value); c.Pop(Value); c.Pop(Value); c.Pop(Value);

    c.Print(); 
    return 0;
}

