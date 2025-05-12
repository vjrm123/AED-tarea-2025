#include<iostream>

struct node {
	int value;
	node* next;
	node(int value, node* next = nullptr, node* prev = nullptr) : value(value), next(next){}
};

struct LE {
	node* head = nullptr;

	void print();
	void add(int value);
	void del(int value);
};

void LE::add(int value) {
	node** current = &head;
	while (*current && (*current)->next != head) current = &((*current)->next);
	node* newNode = new node(value);
	newNode->next = *current ? head : newNode;
	if (!head) head = newNode;
	else (*current)->next = newNode; 
}

void LE::del(int value) {
	if (!head) return;
	if (head->value == value) {
		if (head->next == head) {
			delete head;
			head = nullptr;
		}
		else {
			node* current = head;
			while (current->next != head) { current = current->next;  }
			node* temp = head;
			head = head->next;
			current->next = head;
			delete temp;
		}
	}
	else {
		node* current = head->next;
		node* previos = head;
		while (current != head) {
			if (current->value == value) {
				previos->next = current->next;
				delete current;
				return;
			}
			previos = current;
			current = current->next;
		}
	}
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
	l.add(3); l.add(4); l.add(6); l.add(7); l.add(9); l.add(12); l.add(11);
	l.del(3);
	l.print();
	return 0;
}