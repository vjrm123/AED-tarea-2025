#include <iostream>

struct Node{
    int Value;
    Node* Next;
    Node(int Value, Node* Next = nullptr) : Value(Value), Next(Next) {}
};

struct LE{
    Node* Head = nullptr;
    void Print();
    void Add(int Value);
    void AddPos(int Value, int Pos);
    void Del(int Value);
    void DelPos(int Pos);
    bool Find(int Value, Node*& Pos);
    void Reverse();
    void Sort();
    bool DetectCycle();
    Node* FindMiddle();

    ~LE();
};

void LE::Print(){
    Node* Current = Head;
    std::cout<<"HEAD-> ";
    while(Current) {
        std::cout<< Current->Value << "-> ";
        Current = Current->Next;
    }
    std::cout<<"NULL\n";
}

bool LE::Find(int Value, Node*& Pos) {
    Node** Current = &Head; 
    Node* Prev = nullptr;  
    while (*Current && (*Current)->Value < Value) {
        Prev = *Current;  
        Current = &((*Current)->Next); 
    }
    Pos = Prev;
    return (*Current) && ((*Current)->Value == Value); 
}

void LE::Add(int Value) {
    Node** Current = &Head; 
    while (*Current && (*Current)->Value < Value) { Current = &((*Current)->Next); }
    if (!(*Current) || (*Current)->Value != Value) { 
        Node* NewNode = new Node(Value); 
        NewNode->Next = *Current; 
        *Current = NewNode; 
    }
}

void LE::AddPos(int Value, int Pos) {
    Node** Current = &Head;  
    for (int Index = 0; *Current && Index < Pos-1; ++Index) {
        Current = &((*Current)->Next);
    }
    Node* NewNode = new Node(Value);
    NewNode->Next = *Current;
    *Current = NewNode;
}

void LE::Del(int Value) {
    Node** Current = &Head; 
    while (*Current && (*Current)->Value != Value) { Current = &((*Current)->Next); }
    if (*Current) { 
        Node* Todelete = *Current; 
        *Current = (*Current)->Next; 
        delete Todelete;
    }
}
void LE::DelPos(int Pos) {
    if (Pos < 0) return; 
    Node** Current = &Head; 
    for (int Index = 0; *Current && Index < Pos; ++Index) { Current = &((*Current)->Next); }
    if (*Current) { 
        Node* Todelete = *Current; 
        *Current = (*Current)->Next; 
        delete Todelete; 
    }
}

void LE::Reverse() {
    Node* Prev = nullptr;
    Node* Current = Head;
    Node* Next = nullptr;
    while (Current) { 
        Next = Current->Next; 
        Current->Next = Prev; 
        Prev = Current; 
        Current = Next; 
    }
    Head = Prev; 
}

void LE::Sort() {
    if (!Head || !Head->Next) return; 

    bool Swapped;
    do {
        Swapped = false;
        Node** Current = &Head;
        while ((*Current)->Next) {
            if ((*Current)->Value > (*Current)->Next->Value) { 
                Node* Temp = *Current;
                *Current = (*Current)->Next;
                Temp->Next = (*Current)->Next;
                (*Current)->Next = Temp;
                Swapped = true;
            }
            Current = &((*Current)->Next);
        }
    } while (Swapped);
}

bool LE::DetectCycle() {
    if (!Head) return false; 

    Node* Slow = Head; 
    Node* Fast = Head; 

    while (Fast && Fast->Next) {
        Slow = Slow->Next; 
        Fast = Fast->Next->Next; 

        if (Slow == Fast) { 
            return true;
        }
    }
    return false; 
}

Node* LE::FindMiddle() {
    if (!Head) return nullptr; 

    Node* Slow = Head; 
    Node* Fast = Head; 

    while (Fast && Fast->Next) {
        Slow = Slow->Next; 
        Fast = Fast->Next->Next; 
    }
    return Slow;
}

LE::~LE(){
    while(Head){
        Node* Temp = Head;
        Head = Head->Next;
        delete Temp;
    }
}

int main(){
    LE lista;
    lista.Add(3);
    lista.Add(2);
    lista.AddPos(1,1);
    lista.Del(1);
    lista.Print();
    return 0;
}