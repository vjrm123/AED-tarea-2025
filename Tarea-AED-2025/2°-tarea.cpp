#include <iostream>

template<class T>
struct Node {
    T Value;
    Node<T>* Next;
    Node(T Value, Node<T>* Next = nullptr) : Value(Value), Next(Next) {}
};

template<class T>
struct Pila {
    Node<T>* Top = nullptr;
    int Count = 0;

    void Print();
    bool EstaVacia();
    bool EstaLlena();
    void Push(T Value);
    void Pop();
};

template<class T>
void Pila<T>::Print() {
    Node<T>* Current = Top;
    while (Current) {
        std::cout << Current->Value << " ";
        Current = Current->Next;
    }
    std::cout << "\n";
}

template<class T>
bool Pila<T>::EstaLlena() { return Count >= 10; }

template<class T>
bool Pila<T>::EstaVacia() { return Top == nullptr; }

template<class T>
void Pila<T>::Push(T Value) {
    if (EstaLlena()) { 
        std::cout << "La Pila está llena!!\n"; 
        return; 
    }
    Top = new Node<T>(Value, Top);
    Count++;
}

template<class T>
void Pila<T>::Pop() {
    if (EstaVacia()) { 
        std::cout << "La pila está vacía!!\n"; 
        return;
    }
    Node<T>* temp = Top;
    Top = Top->Next;
    delete temp;
    Count--;
}

int main() {
    Pila<int> Pi;
    

    Pi.Pop();
    Pi.Print(); 

    return 0;
}
