#include<iostream>

template<class T>
class Deque{
    private:
    int size_map = 5;
    int size_block = 5;
    T** Map = nullptr;
    T** head_block = nullptr;
    T** tail_block = nullptr;
    T* head = nullptr;
    T* tail = nullptr;

    void expand_map();
    public:
    Deque();
    ~Deque();

    void Print();
    void pushFront(T value);
    void pop_front();
};

template<class T>
Deque<T>::Deque(){
    Map = new T*[size_map]{nullptr};
    head_block = tail_block = Map + (size_map/2);
}

template<class T>
Deque<T>::~Deque(){
    for(T** ite = Map ; ite < Map + size_map; ++ite ) { if(*ite) delete[] *ite; }
    delete[] Map;
}

template<class T>
void Deque<T>::expand_map(){
    int blockUsados = tail_block - head_block;
    int newSize = size_map * 2 -1;
    T** newMap = new T*[newSize]();

    int newCenter = newSize /2;
    int offset = newCenter - (blockUsados/2);
    for (int i=0; i < blockUsados; i++) { newMap[offset + i] = Map[i]; }

    head_block = Map + offset;
    tail_block = head_block + blockUsados-1;
}

template<class T>
void Deque<T>::pushFront(T value){
    if(!head){
        *head_block = new T[size_block];
        head = tail = *head_block + (size_block /2);
        *head = value;
    }
    else if(head == *head_block){
        if(head_block == Map ) expand_map();
        head_block--;
        *head_block = new T[size_block];
        head = *head_block + size_block-1;
        *head = value;
    }
    else{
        head--;
        *head = value;
    }
}

template<class T>
void Deque<T>::pop_front(){
    if(!head) { std::cout<<"deque vacio!!\n"; }
    if(head == tail){
        delete[] *head_block;
        *head_block = nullptr;
        head = tail = nullptr;
        head_block = tail_block = Map + (size_map/2);
    }
    else if(head == *head_block + size_block-1){
        delete[] *head_block;
        *head_block = nullptr;
        head_block++;
        head = *head_block;
    }
    else{
        head++;
    }

}

template<class T>
void Deque<T>::Print(){
    if(!head) { std::cout<<"esta vacio!!\n"; return; }

    T** Current_block = head_block;
    T* Current = head;
    while(true){
        std::cout<< *Current <<" ";
        if(Current_block == tail_block && Current == tail) break;
        if(Current == *Current_block + size_block-1){
            Current_block++;
            Current = *Current_block;
        }else{
            Current++;
        }
    }
    std::cout<<"\n";
}

template<class T, class container = Deque<T>>
class Stack {
    private:
    container con;
    public:
    Stack();
    ~Stack();

    void push(const T& value);
    void pop();
    T& top();
    void print();
};

template<class T, class container>
Stack<T, container>::Stack() = default;

template<class T, class container>
Stack<T, container>::~Stack() = default;

template<class T, class container>
void Stack<T, container>::push(const T& value){ con.pushFront(value); }

template<class T, class container>
void Stack<T, container>::pop() { con.pop_front(); }

template<class T, class container>
T& Stack<T, container>::top(){ return *(con.head); }

template<class T, class container>
void Stack<T, container>::print() { con.Print(); }

int main(){
    Stack<int, Deque<int>> s;
    s.push(5); s.push(6); s.push(7); s.push(8); s.push(9); s.push(10);
    s.push(11); s.push(12); s.push(13); s.push(14); s.push(15); s.push(16);
    s.pop();

    s.print();
    
    return 0;
}