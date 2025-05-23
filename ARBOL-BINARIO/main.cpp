#include <iostream>
#include <stack>
#include <queue>
#include <utility> // Para std::pair
using namespace std;

struct node {
    int valor;
    node* nod[2];  

    node(int valor) {
        this->valor = valor;
        nod[0] = nod[1] = nullptr;  
    }
};

class cbintree {
public:
    cbintree();
    ~cbintree();
    bool find(int valor, node**& p);
    bool add(int valor);
    bool del(int valor);
    void clear(node* p);  

    // Recorridos
    void inorder(node* p);
    void preorder(node* p);
    void postorder(node* p);
    void inverse_inorder(node* p);
    void level_order();
    
    // Funciones de acceso para los recorridos
    node* getRoot() { return root; }

private:
    node** Rep(node** p); 
    node* root;  
};

// Implementaciones existentes (constructor, destructor, clear, find, add, del, Rep)
cbintree::cbintree() : root(nullptr) {}

cbintree::~cbintree() {
    clear(root);
}

void cbintree::clear(node* p) {
    if (!p) return;
    clear(p->nod[0]);
    clear(p->nod[1]);
    delete p;
}

bool cbintree::find(int valor, node**& p) {
    p = &root;
    while (*p && (*p)->valor != valor) {
        p = &((*p)->nod[(*p)->valor < valor]);
    }
    return *p != nullptr;
}

bool cbintree::add(int valor) {
    node** P;
    if (find(valor, P)) return false;  
    *P = new node(valor);
    return true;
}

node** cbintree::Rep(node** p) {
    node** q = &((*p)->nod[1]);  
    while ((*q)->nod[0]) {  
        q = &((*q)->nod[0]);
    }
    return q;
}

bool cbintree::del(int valor) {
    node** P;
    if (!find(valor, P)) return false;  
    if ((*P)->nod[0] && (*P)->nod[1]) {  
        node** q = Rep(P);  
        (*P)->valor = (*q)->valor;  
        P = q;  
    }
    node* t = *P;
    *P = (*P)->nod[(*P)->nod[1] != nullptr];  
    delete t; 
    return true;
}

// Implementaciones de los recorridos

// Recorrido Inorder (izquierda, raíz, derecha)
void cbintree::inorder(node* p) {
    if (!p) return;
    inorder(p->nod[0]);
    cout << p->valor << " ";
    inorder(p->nod[1]);
}

// Recorrido Preorder (raíz, izquierda, derecha)
void cbintree::preorder(node* p) {
    if (!p) return;
    cout << p->valor << " ";
    preorder(p->nod[0]);
    preorder(p->nod[1]);
}

// Recorrido Postorder (izquierda, derecha, raíz)
void cbintree::postorder(node* p) {
    if (!p) return;
    postorder(p->nod[0]);
    postorder(p->nod[1]);
    cout << p->valor << " ";
}

// Recorrido Inorder inverso (derecha, raíz, izquierda)
void cbintree::inverse_inorder(node* p) {
    if (!p) return;
    inverse_inorder(p->nod[1]);
    cout << p->valor << " ";
    inverse_inorder(p->nod[0]);
}

// Recorrido por niveles (usando BFS con cola)
void cbintree::level_order() {
    if (!root) return;
    
    queue<node*> q;
    q.push(root);
    
    while (!q.empty()) {
        node* current = q.front();
        q.pop();
        cout << current->valor << " ";
        
        if (current->nod[0]) q.push(current->nod[0]);
        if (current->nod[1]) q.push(current->nod[1]);
    }
}



int main() {
    cbintree tree;
    tree.add(10);
    tree.add(5);
    tree.add(15);
    tree.add(12);
    tree.add(7);
    tree.add(3);
    tree.add(17);

    cout << "Inorder: ";
    tree.inorder(tree.getRoot());
    cout << endl;

    cout << "Preorder: ";
    tree.preorder(tree.getRoot());
    cout << endl;

    cout << "Postorder: ";
    tree.postorder(tree.getRoot());
    cout << endl;

    cout << "Inverse Inorder: ";
    tree.inverse_inorder(tree.getRoot());
    cout << endl;

    cout << "Level Order: ";
    tree.level_order();
    cout << endl;

    return 0;
}