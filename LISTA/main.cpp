#include <iostream>

template<class T>
struct Node{
    T value;
    Node* Next, *Prev;
    Node(T value, Node* Next = nullptr, Node* Prev = nullptr) : value(value), Next(Next), Prev(Prev) {}
};

template<class T>
struct Lista{
    Node<T>* Head;
    Node<T>* Tail;
    int Size;
    Lista();

    void Push_back(const T& value);
    void Push_front(const T& value);
    void Pop_back();
    void Pop_front();
    void Print() const;
    T& operator[](int Index);

    //~Lista();
};

template<class T>
Lista<T>::Lista(): Head(nullptr), Tail(nullptr), Size(0) {}

template<class T>
void Lista<T>::Print()const {
    std::cout<<"HEAD-> ";
    Node<T>* Current = Head;
    while(Current) {
        std::cout<<Current->value<<"-> ";
        Current = Current->Next;
    }
    std::cout<<"NULL\n";
}

template<class T>
void Lista<T>::Push_front(const T& Value){
    Node<T>* NewNode = new Node<T>(Value);
    if(!Head) { Head = Tail = NewNode; }
    else {
        NewNode->Next = Head;
        Head->Prev = NewNode;
        Head = NewNode;
    }
    Size++;
}

template<class T>
void Lista<T>::Push_back(const T& Value){
    Node<T>* NewNode = new Node<T>(Value);
    if(!Head) { Head = Tail = NewNode; }
    else {
        Tail->Next = NewNode;
        NewNode->Prev = Tail;
        Tail = NewNode;
    }
}

template<class T>
void Lista<T>::Pop_front(){
    if(Node<T>* Temp = Head){
        (Head = Head->Next) ? Head->Prev = nullptr : Tail = nullptr;
        delete Temp;
        Size--;
    }
}

template<class T>
void Lista<T>::Pop_back(){
    if(Node<T>* Temp = Tail){
        (Tail = Tail->Prev) ? Tail->Next = nullptr : Head = nullptr;
        delete Temp;
        Size--;
    }
}

template<class T>
T& Lista<T>::operator[](int index) {
    if (index < 0 || index >= Size) std::cout<<"ERROR!!";
    
    Node<T>* temp = (index <= Size/2) ? Head : Tail;
    int steps = (index <= Size/2) ? index : Size - 1 - index;
    
    while (steps--) temp = (index <= Size/2) ? temp->Next : temp->Prev;
    
    return temp->value;
}

int main(){
    Lista<int> lista;
    lista.Push_front(5);
    lista.Push_front(4);
    lista.Push_back(6);
    lista.Push_back(7);
    //lista.Pop_front();
    //lista.Pop_back();
    lista[1] = 100;
    lista.Print();
    return 0;
}