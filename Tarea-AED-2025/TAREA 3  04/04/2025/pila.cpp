#include<iostream>

template<class T>
struct Node {
	T elements[5];
	int count = 0;
	Node* Previos = NULL;
};

template<class T>
struct Pila {
	Node<T>* Top = NULL;

	void Push(T Value);
	void Pop(T& value);
	void Print();
	void Print1();

	~Pila();
};

template<class T>
void Pila<T>::Push(T Value) {
	if (!Top || Top->count == 5) {
		Node<T>* NewNode = new Node<T>;
		NewNode->Previos = Top;
		Top = NewNode;
	}
	Top->elements[Top->count++] = Value;
}

template<class T>
void Pila<T>::Pop(T& value) {
	if (!Top || Top->count == 0) { std::cout << "esta vacia!!\n"; return; }
	value = Top->elements[--Top->count];
	if (Top->count == 0) {
		Node<T>* ToDelete = Top;
		Top = Top->Previos;
		delete ToDelete;
	}
}
template<class T>
void Pila<T>::Print() {
	std::cout << "[ ";
	bool First = true;
	Node<T>* Current = Top; 

	while (Current) { 
		std::cout << (First ? "[ " : "-> [");
		First = false;
		for (int i = Current->count - 1, c = 0; i >= 0; --i) { 
			std::cout << (c++ ? " " : "") << Current->elements[i]; 
		}
		std::cout << " ]";
		Current = Current->Previos;
	}
	std::cout << " ]\n";
}

template<class T>
void Pila<T>::Print1() {
	std::cout << "[ ";
	bool First = true;
	Node<T>* Current = Top;
	while (Current) {
		for (int i = Current->count - 1; i >= 0; --i) {
			std::cout << (First ? "" : ",") << Current->elements[i];
			First = false;
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

	Pila<int> p; int Value;
	p.Push(1); p.Push(2); p.Push(3); p.Push(4); p.Push(5); 
	p.Push(6); p.Push(7); p.Push(8); p.Push(9); p.Push(10); 
	p.Push(11);

	p.Print();
	
	p.Pop(Value);

	p.Print();

	return 0;
}
