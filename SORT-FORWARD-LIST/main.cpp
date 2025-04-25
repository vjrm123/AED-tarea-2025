#include<iostream>

template<class T>
struct node {
    T value;
    node<T>* next;
    node(T value, node<T>* next = nullptr) : value(value), next(next) {}
};

template<class T>
struct sort_forward_list {
    node<T>* head = nullptr;

    void insert(T value);
    void remove(T value);
    bool find(T value, node<T>*& pos);
    void print();

    ~sort_forward_list();
};

template<class T>
void sort_forward_list<T>::insert(T value) {
    node<T>** pos = &head;
    while (*pos && (*pos)->value < value) { pos = &((*pos)->next); }
    if(!(*pos) || (*pos)->value != value){ *pos = new node<T>(value, *pos); }
}

template<class T>
void sort_forward_list<T>::remove(T value){
    node<T>** temp = &head;
    while(*temp && (*temp)->value < value){ temp = &((*temp)->next); }
    if(*temp && (*temp)->value == value){
        node<T>* toDelete = *temp;
        *temp = (*temp)->next;
        delete toDelete;
    }
}

template<class T>
bool sort_forward_list<T>::find(T value, node<T>*& pos){
    node<T>** current = &head;
    while(*current && (*current)->value < value) { current = &((*current)->next); }
    pos = *current;
    return *current && (*current)->value == value;
}

template<class T>
void sort_forward_list<T>::print() {
    node<T>* current = head;
    while (current) {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

template<class T>
sort_forward_list<T>::~sort_forward_list() {
    while (head) {
        node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    sort_forward_list<int> s;
    s.insert(4); s.insert(98); s.insert(3); s.insert(4);
    s.insert(2);
    s.insert(5);
    s.insert(1);
    s.remove(1);
    s.print();  
    return 0;
}