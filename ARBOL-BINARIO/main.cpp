#include <iostream>
using namespace std;

struct node {
    int valor;
    node* nod[2];  

    node(int valor) {
        this->valor = valor;
        nod[0] = nod[1] = 0;  
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

private:
    node** Rep(node** p); 
    node* root;  
};


cbintree::cbintree() : root(NULL) {}


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
    return *p != 0;
}

bool cbintree::add(int valor) {
    node** P;
    if (find(valor, P)) return 0;  
    *P = new node(valor);
    return 1;
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
    if (!find(valor, P)) return 0;  
    if ((*P)->nod[0] && (*P)->nod[1]) {  
        node** q = Rep(P);  
        (*P)->valor = (*q)->valor;  
        P = q;  
    }
    node* t = *P;
    *P = (*P)->nod[(*P)->nod[1] != 0];  
    delete t; 
    return 1;
}

int main() {
    cbintree tree;
    tree.add(10);
    tree.add(5);
    tree.add(15);
    tree.add(12);

    cout << "Delete 10: " << tree.del(10) << endl;  
    cout << "Delete 5: " << tree.del(5) << endl;    
    return 0;
}
