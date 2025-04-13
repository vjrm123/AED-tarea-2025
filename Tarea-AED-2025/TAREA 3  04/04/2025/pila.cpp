#include <iostream>

template<class T>
struct Node {
    T elements[5];
    T* top = elements;       
    Node* Previos = nullptr; 
};

template<class T>
struct Pila {
    Node<T>* Top = nullptr;

    void Push(T Value);
    void Pop(T& value);
    void Print();

    ~Pila();
};

template<class T>
void Pila<T>::Push(T Value) {
    if (!Top || Top->top == Top->elements + 5) { 
        Node<T>* NewNode = new Node<T>;
        NewNode->Previos = Top;
        Top = NewNode;
    }
    *(Top->top++) = Value; 
}

template<class T>
void Pila<T>::Pop(T& value) {
    if (!Top || Top->top == Top->elements) { std::cout << "Esta vacia!!\n"; return; }
    value = *(--Top->top); 
    if (Top->top == Top->elements) { 
        Node<T>* ToDelete = Top;
        Top = Top->Previos;
        delete ToDelete;
    }
}

template<class T>
void Pila<T>::Print() {
    std::cout << "[";
    bool FirstElement = true;
    Node<T>* Current = Top;
    while (Current) {
        for (T* p = Current->top - 1; p >= Current->elements; --p) {
            std::cout << (FirstElement ? "" : ", ") << *p;
            FirstElement = false;
        }
        Current = Current->Previos;
    }
    std::cout << "]\n";
}

template<class T>
Pila<T>::~Pila() {
    T value;
    while (Top) { Pop(value); }
}

int main() {
    Pila<int> p;
    int Value;
    p.Push(1); p.Push(2); p.Push(3); p.Push(4); p.Push(5);
    p.Push(6); p.Push(7); p.Push(8); p.Push(9); p.Push(10); p.Push(11);

    p.Print();  
    p.Pop(Value);
    p.Print();  

    p.Pop(Value); p.Pop(Value); p.Pop(Value); p.Pop(Value); p.Pop(Value);
    p.Pop(Value); p.Pop(Value); p.Pop(Value); p.Pop(Value); p.Pop(Value); p.Pop(Value);

    return 0;
}