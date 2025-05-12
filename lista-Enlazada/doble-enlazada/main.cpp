#include<iostream>
using namespace std;

struct node {
	int value;
	node* next;
	node* prev;
	node(int value, node* next = nullptr, node* prev = nullptr) : value(value), next(next), prev(prev) {}
};

struct LE {
	node* head = nullptr;

	void print();
	void add(int);
	void del(int);
	void ImparPar();
	void parImpar();
};

void LE::add(int value) {
	node** curr = &head;
	while (*curr && (*curr)->value < value) { curr = &((*curr)->next); }
	if (!(*curr) || (*curr)->value != value) {
		node* newNode = new node(value);
		newNode->next = *curr;
		newNode->prev = (*curr) ? (*curr)->prev : nullptr;
		if (*curr) (*curr)->prev = newNode;
		*curr = newNode;
 	}
}

void LE::del(int value) {
	for (node** ite = &head; *ite; ite = &((*ite)->next)) {
		if ((*ite)->value == value) {
			node* temp = *ite;
			*ite = temp->next;
			if (*ite) (*ite)->prev = temp->prev;
			delete temp;
			break;
		}
	}
}

void LE::ImparPar() {
	node* impar = head;
	node* par = head->next;
	node* headpar = par;

	while (par && par->next) {
		impar->next = par->next;
		if (par->next) {
			par->next->prev = impar;
		}
		impar = impar->next;

		par->next = impar->next;
		if (impar->next) {
			impar->next->prev = par;
		}
		par = par->next;
	}
	impar->next = headpar;
	if(headpar) {
		head->prev = impar;
	}
}

void LE::parImpar() {
	if (!head || !head->next) return;
	node* impar = head;
	node* par = head->next;
	node* headImpar = impar;
	node* headPar = par;
	while (par && par->next) {
		impar->next = par->next;
		if (par->next) {
			par->next->prev = impar;
		}
		impar = impar->next;

		par->next = impar->next;
		if (impar->next) {
			impar->next->prev = par;
		}
		par = par->next;
	}
	head = headPar;
	if (headPar) {
		node* lastPar = headPar;
		while (lastPar->next) lastPar = lastPar->next;
		lastPar->next = headImpar;
		if (headImpar) {
			headImpar->prev = par;
		}
		if (impar) {
			impar->next = nullptr;
		}
	}
}

void LE::print() {
	if (!head) {
		std::cout << "Lista vacía" << std::endl;
		return;
	}

	node* current = head;
	while (current) {
		std::cout << current->value << " ";
		current = current->next;
	}
	std::cout << std::endl;
}

int main() {

	LE l;
	l.add(1); l.add(2); l.add(3); l.add(4); l.add(5); l.add(6); l.add(7); l.add(8);
	l.print();
	l.parImpar();
	//l.ImparPar();
	l.print();

	return 0;
}