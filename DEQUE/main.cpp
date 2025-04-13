#include <iostream>

template<class T>
class Deque{
    private:
        int Map_capacity = 7;
        int Size_block = 5;

        T** Map = nullptr;
        T** Head_block = nullptr;
        T** Tail_block = nullptr;
        T* Head = nullptr;
        T* Tail = nullptr;

        void expand_map();
    public:
        Deque();
        ~Deque();

        void Push_back(T value);
        void Push_front(T value);
        void Pop_back();
        void Pop_front();
        T& operator[](int index);
        bool empty() const { return Head == nullptr; }
        void Print();
};

template<class T>
Deque<T>::Deque() {
    Map = new T*[Map_capacity]{nullptr};
    Head_block = Tail_block = Map + (Map_capacity / 2);
}

template<class T>
Deque<T>::~Deque(){
    for(T** it = Map; it < Map + Map_capacity; ++it) { if(*it) delete[] *it; }
    delete[] Map;
}

template<class T>
void Deque<T>::expand_map(){
    int NewCapacity = Map_capacity * 2-1;
    T** NewMap = new T*[NewCapacity]();
    int Offset = (NewCapacity - Map_capacity)/2;

    for(int i = 0; i < Map_capacity; ++i) { NewMap[Offset+i] = Map[i]; }

    Head_block = NewMap + (Head_block - Map) + Offset;
    Tail_block = NewMap + (Tail_block - Map) + Offset;

    delete[] Map;
    Map = NewMap;
    Map_capacity = NewCapacity;
}

template<class T>
void Deque<T>::Push_back(T value){
    if(!Tail){
        *Tail_block = new T[Size_block];
        Head = Tail = *Tail_block + (Size_block/2);
        *Tail = value;
    } else if(Tail == *Tail_block + Size_block-1){
        if(Tail_block == Map + Map_capacity-1) expand_map();
        Tail_block++;
        *Tail_block = new T[Size_block];
        Tail = *Tail_block;
        *Tail = value;
    }else{
        Tail++;
        *Tail = value;
    }
}

template<class T>
void Deque<T>::Push_front(T value){
    if(!Head){
        *Head_block = new T[Size_block];
        Head = Tail = *Head_block + (Size_block/2);
        *Head = value;
    }else if(Head == *Head_block){
        if(Head_block == Map) expand_map();
        Head_block--;
        *Head_block = new T[Size_block];
        Head = *Head_block + Size_block-1;
        *Head = value;
    }else{
        Head--;
        *Head = value;
    }
}

template<class T>
void Deque<T>::Pop_back(){
    if(!Tail){ std::cout<<"esta Vacio!!\n"; }
    if(Tail == Head){
        delete[] *Tail_block;
        *Tail_block = nullptr;
        Head = Tail = nullptr;
        Head_block = Tail_block = Map + (Map_capacity/2);
    }
    else if(Tail == *Tail_block){
        delete[] *Tail_block;
        Tail_block--;
        Tail = *Tail_block + Size_block-1;
    }
    else {
        Tail--;
    }
}

template<class T>
void Deque<T>::Pop_front(){
    if(!Head) std::cout<<"esta vacio!!\n";
    if(Head == Tail){
        delete *Head_block;
        *Head_block = nullptr;
        Head = Tail = nullptr;
        Head_block = Map + (Map_capacity/2);
    }else if(Head == *Head_block+ Size_block-1){
        delete *Head_block;
        Head_block--;
        Head = *Head_block + Size_block-1;
    }
    else{
        Head++;
    }
}

template<class T>
T& Deque<T>::operator[](int Index){
    if(empty()) std::cout<<"deque vacio!!\n";
    int Total_pos = (Head - *Head_block) + Index;
    int block_offset = Total_pos / Size_block;
    int elem_offset = Total_pos % Size_block;

    T** Target_block = Head_block + block_offset;
    return (*Target_block)[elem_offset];
}

template<class T>
void Deque<T>::Print(){
    if(empty()) { std::cout<<"esta vacio!!\n"; return; }

    T** Current_block = Head_block;
    T* Current = Head;
    while(true){
        std::cout<< *Current <<" ";
        if(Current_block == Tail_block && Current == Tail) break;
        if(Current == *Current_block + Size_block-1){
            Current_block++;
            Current = *Current_block;
        }else{
            Current++;
        }
    }
    std::cout<<"\n";
}

int main(){
    Deque<int> d;
    d.Push_back(5); d.Push_back(6); d.Push_back(7); d.Push_back(8); d.Push_back(9); d.Push_back(10);
    d.Push_front(4); d.Push_front(3); d.Push_front(2); d.Push_front(1); d.Push_front(0);
    d.Pop_back();
    d.Pop_front();
    d[8] = 100;
    d.Print();
    return 0;
}