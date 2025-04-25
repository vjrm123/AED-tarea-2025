#include <iostream>

template<class T>
struct Pila {
    T elements[10];         
    T* top = nullptr;       

    void Push(T valor);
    void Pop(T& valor);
    void Print();
};

template<class T>
void Pila<T>::Push(T valor) {
    if (top == elements + 9) { std::cout << "Pila llena\n"; return; }
    top = (top == nullptr) ? elements : top + 1;
    *top = valor;
}

template<class T>
void Pila<T>::Pop(T& valor) {
    if (top == nullptr) { std::cout << "Pila vacia\n"; return; }
    valor = *top;
    top = (top == elements) ? nullptr : top - 1;
}

template<class T>
void Pila<T>::Print() {
    std::cout << "[ ";
    for (T* p = top; p >= elements; --p) { std::cout << *p << " "; }
    std::cout << "]\n";
}


int main() {
    Pila<int> p;
    int valor;

    p.Push(1); p.Push(2); p.Push(3); p.Push(4); p.Push(5); p.Push(6);
    p.Push(7); p.Push(8); p.Push(9); p.Push(10); 
    p.Print(); 

    p.Push(11);

    p.Pop(valor);
    p.Print(); 

    p.Push(11);
    p.Print(); 

    p.Pop(valor); p.Pop(valor); p.Pop(valor); p.Pop(valor); p.Pop(valor); 
    p.Pop(valor); p.Pop(valor); p.Pop(valor); p.Pop(valor); p.Pop(valor); p.Pop(valor);

    return 0;
}