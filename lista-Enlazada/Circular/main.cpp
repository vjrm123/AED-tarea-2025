#include<iostream>
#include<stack>

struct node {
	int value;
	node* next;
	node(int value, node* next = nullptr):value(value), next(next){}
};

struct LE {
	node* head = nullptr;

	void add(int);
	void del(int);
	void print();
	bool esSimetrica();
};

void LE::add(int value) {
	node** current = &head;
	while (*current && (*current)->next != head) { current = &((*current)->next); }
	node* newNode = new node(value);
	newNode->next = (*current) ? head : newNode;
	if (!head) head = newNode;
	else (*current)->next = newNode;
}

void LE::del(int value) {
	if (head->value == value) {
		if (head->next == head) {
			delete head;
			head = nullptr;
		}
		else {
			node* current = head;
			while (current->next != head) { current = current->next; }
			node* temp = head;
			head = head->next;
			current->next = head;
			delete temp;
		}
	}
	else {
		node* previo = head;
		node* current = head->next;
		while (current != head) {
			if (current->value == value) {
				previo->next = current->next;
				delete current;
				return;
			}
			previo = current;
			current = current->next;
		}
	}
}


bool LE::esSimetrica() {
	if (!head || head->next == head) return true;
	std::stack<int> pila;
	node* lento = head;
	node* rapido = head;
	do
	{
		pila.push(lento->value);
		lento = lento->next;
		rapido = rapido->next->next;
	} while (rapido != head && rapido->next != head);

	if (rapido->next == head) lento = lento->next;
	while (lento != head) {
		if (pila.top() != lento->value)return false;
		pila.pop();
		lento = lento->next;
	}
	return true;
}

void LE::print() {
	
	if (!head) {
		std::cout << "Lista vacía" << std::endl;
		return;
	}

	node* current = head;
	do {
		std::cout << current->value << " ";
		current = current->next;
	} while (current != head);
	std::cout << std::endl;
}

int main() {
	LE l;
	l.add(1); l.add(5); l.add(5); l.add(1);
	//l.del(4);
	l.print();
	std::cout << l.esSimetrica();
	return 0;
}