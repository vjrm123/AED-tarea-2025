#include <iostream>

template<class T>
struct Vector{
    T* data;
    int size;
    int Capacity;

    Vector();
    void Resize(int NewCapacity);

    void Push_back(const T& Value);
    void Pop_Back();
    void Push_front(const T& value);
    void Pop_front();
    void Print();

    T& operator[](int index);
    const T& operator[](int index)const;

    ~Vector();

};

template<class T>
Vector<T>::Vector() : data(nullptr), size(0), Capacity(1) { data = new T[Capacity]; }

template<class T>
void Vector<T>::Print(){
    std::cout<<"[ ";
    for (int i = 0; i < size; ++i){
        std::cout<< data[i];
        if(i < size-1){ std::cout<<", "; }
    }
    std::cout<<" ]\n";
}

template<class T>
void Vector<T>::Resize(int NewCapacity){
    T* NewData = new T[NewCapacity];
    for(int i = 0; i < size; ++i){
        NewData[i] = data[i];
    }
    delete []data;
    data = NewData;
    Capacity = NewCapacity;
}

template<class T>
void Vector<T>::Push_back(const T& Value){
    if(size >= Capacity){
        Resize(2*Capacity);
    }
    data[size++] = Value;
}

template<class T>
void Vector<T>::Push_front(const T& Value){
    if(size >= Capacity){
        Resize(2*Capacity);
    }
    for(int i = size; i > 0; --i){
        data[i] = data[i-1];
    }
    data[0] = Value;
    ++size;
}

template<class T>
void Vector<T>::Pop_Back(){
    if(size > 0){
        --size;
        data[size].~T();
    }
}

template<class T>
void Vector<T>::Pop_front(){
    if(size == 0) return;
    data[0].~T();
    for(int i=0; i < size-1; ++i){
        data[i] = data[i+1];
    }
    --size;
}

template<class T>
T& Vector<T>::operator[](int index){
    return data[index];
}

template<class T>
const T& Vector<T>::operator[](int index) const {
    return data[index];
}

template<class T>
Vector<T>::~Vector(){
    delete []data;
}

int main(){
    Vector<int> vec;
    vec.Push_back(3);
    vec.Push_back(4);
    vec.Push_front(2);
    vec.Push_front(1);
    vec.Pop_Back();
    vec.Pop_front();
    vec[0] = 0;
    vec.Print();
    return 0;
}