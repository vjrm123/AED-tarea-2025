#include <iostream>

template<class T>
struct Cola {
	T element[10];
	T* Tail, * Head = nullptr;

	Cola();
	T* Next(T* Ptr);
	bool Vacia();
	bool Llena();

	void Push(T value);
	void Pop(T& value);
	void Print();

	~Cola();
};

template<class T>
T* Cola<T>::Next(T* Ptr) { return (Ptr == element + 9) ? element : Ptr + 1; }

template<class T>
Cola<T>::Cola() { Head = Tail = nullptr;  }

template<class T>
bool Cola<T>::Vacia() { return Head == nullptr; }

template<class T>
bool Cola<T>::Llena() { return Head != nullptr && Next(Tail) == Head; }

template<class T>
void Cola<T>::Push(T value) {
	if (Llena()) { std::cout << "esta llena!!\n"; return; }
	if (Vacia()) {
		Head = Tail = element;
		*Tail = value;
	}
	else {
		Tail = Next(Tail);
		*Tail = value;
	}
}

template<class T>
void Cola<T>::Pop(T& value) {
	if (Vacia()) { std::cout << "Vacia!!\n"; return; }
	value = *Head;
	if (Head == Tail) { Head = Tail = nullptr;  }
	else { Head = Next(Head); }
}

template<class T>
void Cola<T>::Print() {
	if (Vacia()) {
		std::cout << "[ ]\n";
		return;
	}

	T* Temp = Head;
	std::cout << "[ ";
	do {
		std::cout << *Temp << " ";
		if (Temp == Tail) break;
		Temp = Next(Temp);
	} while (true);
	std::cout << "]\n";
}
template<class T>
Cola<T>::~Cola() {
	T value;
	while (!Vacia()) Pop(value);
}

int main() {

	Cola<int> c;
	int Value;
	c.Push(1); c.Push(2); c.Push(3); c.Push(4);
	c.Push(5); c.Push(6); c.Push(7); 
	c.Push(8); c.Push(9); c.Push(10);
	c.Print();
    
	c.Push(11);

	c.Pop(Value);

	c.Print();

	c.Push(11);
	c.Print();

	return 0;
}