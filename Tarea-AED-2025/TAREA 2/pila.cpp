#include<iostream>

template<class T>
struct Pila {
	T elements[10];
	T* Top = nullptr;
	int Size = 0;

	void Push(T value);
	void Pop(T& value);
	void Print();

	~Pila();
};

template<class T>
void Pila<T>::Push(T Value) {
	if (Size == 10) { std::cout << "llena!!\n"; return; }
	elements[Size] = Value;
	Top = &elements[Size];
	Size++;
}
template<class T>
void Pila<T>::Pop(T& value) {
	if (Size == 0) { std::cout << "Vacia!!\n"; return; }
	value = *Top;
	Size--;
	Top = (Size > 0) ? &elements[Size - 1] : nullptr;
}

template<class T>
void Pila<T>::Print() {
	std::cout << "[ ";
	for (int i = 0; i < Size; ++i) { std::cout << elements[i] << " "; }
	std::cout << "]\n";
}
template<class T>
Pila<T>::~Pila() {
	T value;
	while (Size > 0) { Pop(value); }
}

int main() {
	Pila<int>p;
	int Value;
	p.Push(1); p.Push(2); p.Push(3); p.Push(4); p.Push(5); 
	p.Push(6); p.Push(7); p.Push(8); p.Push(9); p.Push(10);
	p.Print();

	p.Push(11);
	
	p.Pop(Value);
	p.Print();

	p.Push(11);
	p.Print();

	p.Push(12);

	return 0;
}