#include<iostream>

template<class T>
class Deque{
    private:
    int sizeMap = 5;
    int sizeBlock = 5;
    T** Map = nullptr;
    T** head_block = nullptr;
    T** tail_block = nullptr;
    T* head = nullptr;
    T* tail = nullptr;

    void expand_map();

    public:
    Deque();
    ~Deque();

    void print();
    void push_back(const T& value);
    void pop_front();
};

template<class T>
Deque<T>::Deque(){
    Map = new T*[sizeMap]{nullptr};
    head_block = tail_block = Map + (sizeMap/2);
}

template<class T>
Deque<T>::~Deque(){
    for(T** ite = Map; ite < Map + sizeMap; ++ite) { if(*ite) delete[] *ite; }
    delete[] Map;
}

template<class T>
void Deque<T>::expand_map(){
    int blockUsados = tail_block - head_block;
    int newSize = sizeMap * 2 -1;
    T** newMap = new T*[newSize]();

    int newCenter = newSize /2;
    int offset = newCenter - (blockUsados/2);
    for(int i = 0; i < blockUsados; i++){ newMap[offset + i] = head_block[i]; }

    head_block = Map + offset;
    tail_block = head_block + blockUsados-1;
}

template<class T>
void Deque<T>::push_back(const T& value){
    if(!tail){
        *tail_block = new T[sizeBlock];
        head = tail = *tail_block + (sizeBlock/2);
        *tail = value;
    }
    else if(tail == *tail_block + sizeBlock-1){
        if(tail_block == Map + sizeMap-1) expand_map();
        tail_block++;
        *tail_block = new T[sizeBlock];
        tail = *tail_block;
        *tail = value;
    }
    else {
        tail++;
        *tail = value;
    }
}

template<class T>
void Deque<T>::print(){
    if(!head){ std::cout<<"deque vacio!!\n"; }
    T** currentBlock = head_block;
    T* current = head;
    while(true){
        std::cout<< *current <<" ";
        if(currentBlock == tail_block && current == tail) break;
        if(current == *currentBlock + sizeBlock-1){
            currentBlock++;
            current = *currentBlock;
        }
        else{
            current++;
        }
    }
    std::cout<<"\n";
}

template<class T>
void Deque<T>::pop_front(){
    if(!head) { std::cout<<"deque vacio!!\n"; }
    if(head == tail){
        delete[] *head_block;
        *head_block = nullptr;
        head = tail = nullptr;
        head_block = tail_block = Map + (sizeMap/2);
    }
    else if(head == *head_block + sizeBlock-1 ){
        delete[] *head_block;
        *head_block = nullptr;
        head_block++;
        head = *head_block;
    }
    else{
        head++;
    }
}

template<class T, class container = Deque<T>>
class Queue{
    private:
    container con;
    public:
    Queue();
    ~Queue();

    void push(const T& value);
    void pop();
    void print();
};

template<class T, class container>
Queue<T, container>::Queue() = default;

template<class T, class container>
Queue<T, container>::~Queue() = default;

template<class T, class container>
void Queue<T, container>::push(const T& value){ con.push_back(value); }

template<class T, class container>
void Queue<T, container>::pop() { con.pop_front(); }

template<class T, class container>
void Queue<T, container>::print(){ con.print(); }


int main(){
    Queue<int, Deque<int>> q;
    q.push(5); q.push(6); q.push(7); q.push(8); q.push(9); q.push(10); q.push(11);
    q.print();

    q.pop(); q.pop(); q.pop(); q.pop(); q.pop(); q.pop(); q.pop(); q.pop();
    return 0;
}