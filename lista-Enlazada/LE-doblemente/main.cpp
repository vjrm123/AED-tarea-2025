#include<iostream>

struct node {
	int value;
	node* next;
	node* previos;
	node(int value) : value(value), next(nullptr), previos(nullptr){}
};

struct LEdo {
	node* head = nullptr;

	void add(int value);
	void del(int value);
	void print();
};

void LEdo::add(int value) {
    node** current = &head;
    node* last = nullptr;  // Puntero auxiliar seguro
    
    while (*current) {
        last = *current;  // Mantenemos referencia al último nodo válido
        current = &(*current)->next;
    }
    
    *current = new node(value);
    (*current)->previos = last ? last : *current;  // Seguro para todos los casos
}
void LEdo::del(int value) {
	node** current = &head;
	while (*current) {
		if ((*current)->value == value) {
			node* toDelete = *current;
			*current = toDelete->next;
			if (*current) (*current)->previos = toDelete->previos;
			delete toDelete;
		
		}
		else {
			current = &((*current)->next);
		}
	}
}

void LEdo::print() {
	node* current = head;
	while (current) {
		std::cout << current->value << " ";
		current = current->next;
	}
}

int main() {
	LEdo l;
	l.add(3);  // Lista: 3 (previos apunta a sí mismo)
l.add(1);  // Lista: 1 → 3 (previos de 3 no se actualiza correctamente)
l.add(2);
	
	
	l.print();
	return 0;
}