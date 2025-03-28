#include <iostream>

struct Node {
    int Value;
    Node* Next;
    Node(int Value, Node* Next = nullptr) : Value(Value), Next(Next) {}
};

struct LE{
    Node* Head = nullptr;

    void Print();
    void Add(int Value);
    void AddEfi(int Value);
    void AddPos(int Value, int Pos);
    bool Find(int Value, Node*& Pos);
    void AddPosi(int Value, int Pos);
    void del(int Value);
    void Delete(int Value);
    void delPos(int Pos);
    void DelPosEfi(int Pos);
    void Reverse();
    void Sort();

    ~LE();
};

void LE::Print() {
    std::cout<<"HEAD-> ";
    Node* Current = Head;
    while(Current) {
        std::cout<< Current->Value << "-> ";
        Current = Current->Next;
    }
    std::cout<<"NULL\n";
}

bool LE::Find(int Value, Node*& Pos) {
    Node* Current = Head;
    Node* Previo = nullptr;
    while(Current && Current->Value < Value ) {
        Previo = Current;
        Current = Current->Next;
    }
    Pos = Previo;
    return Current && (Current->Value == Value);
}

void LE::Add(int Value){
    Node* NewNode = new Node(Value);
    Node* Pos = nullptr;
    if(!Find(Value, Pos)){
        if(!Pos) {
            NewNode->Next = Head;
            Head = NewNode;
        } else {
            NewNode->Next = Pos->Next;
            Pos->Next = NewNode;
        }
    }
}

void LE::AddEfi(int Value) {
    Node** Current = &Head;
    while(*Current && (*Current)->Value < Value ) { Current = &((*Current)->Next); }
    if(!(*Current) || (*Current)->Value != Value){
        Node* NewNode = new Node(Value);
        NewNode->Next = *Current;
        *Current = NewNode;
    }
}

void LE::AddPos(int Value, int Pos){
    if(Pos < 0) return;
    Node* newNode = new Node(Value);
    if(Pos == 0){
        newNode->Next = Head;
        Head = newNode;
    }
    Node* Current = Head;
    int Index = 0;
    while(Current && Index < Pos-1 ){
        Index++;
        Current = Current->Next;
    }
    newNode->Next = Current->Next;
    Current->Next = newNode;
}

void LE::AddPosi(int Value, int Posi){
    Node** Current = &Head;
    for( int Index = 0; *Current && (*Current)->Value < Posi-1; ++Index )  { Current = &((*Current)->Next); }
    Node* NewNode = new Node(Value);
    NewNode->Next = *Current;
    *Current = NewNode;
}

void LE::del(int Value) {
    Node* Pos = nullptr;
    if(Find(Value, Pos)) {
        Node* ToDelete = nullptr;
        if(!Pos) {
            if(Head == Head->Next){
                ToDelete = Head;
                Head = nullptr;
            } else{
                ToDelete = Head;
                Head = Head->Next;
            }
        } else{
            ToDelete = Pos->Next;
            Pos->Next = ToDelete->Next;
        }
        delete ToDelete;
    }
}

void LE::Delete(int Value) {
    Node** Current = &Head;
    while(*Current && (*Current)->Value != Value ) { Current = &((*Current)->Next); }
    if(*Current) {
        Node* ToDelete = *Current;
        *Current = ToDelete->Next;
        delete ToDelete;
    }
}

void LE::delPos(int Pos){
    if(Pos < 0) return;
    Node* ToDelete = nullptr;
    if(Pos == 0){
        ToDelete = Head;
        Head = Head->Next;
    }else {
        Node* Current = Head;
        int Index = 0;
        while(Current && Index < Pos - 1) {
            Current = Current->Next;
            Index++;
        }
        ToDelete = Current->Next;
        Current->Next = ToDelete->Next;
    }
    delete ToDelete;
}

void LE::DelPosEfi(int Pos) {
    if(Pos < 0 ) return;
    Node** Current = &Head;
    for(int Index = 0; *Current && Index < Pos; ++Index ) { Current = &((*Current)->Next); }
    if(*Current) {
        Node* Todelete = *Current;
        *Current = Todelete->Next;
        delete Todelete;
    }
}

void LE::Reverse(){
    Node* Prev , *Next = nullptr;
    Node* Current = Head;
    while(Current){
        Next = Current->Next;
        Current->Next = Prev;
        Prev = Current;
        Current = Next;
    }
    Head = Prev;
}

void LE::Sort() {
    if (!Head || !Head->Next) return; // Si la lista está vacía o tiene un solo nodo

    bool Swapped;
    do {
        Swapped = false;
        Node** Current = &Head;
        while ((*Current)->Next) {
            if ((*Current)->Value > (*Current)->Next->Value) { // Comparar valores
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

LE::~LE(){
    while(Head){
        Node* Temp = Head;
        Head = Head->Next;
        delete Temp;
    }
}


int main(){
    LE lista;
    lista.Add(4);
    lista.Add(3);
    lista.AddEfi(0);
    lista.AddEfi(6);
    lista.AddPos(5,2);
    lista.AddPosi(2, 0);
    lista.AddPosi(1, 0);
    //lista.delPos(6);
    //lista.DelPosEfi(6);
    lista.Print();
    lista.Reverse();
    lista.Sort();

    lista.Print();
    return 0;
}