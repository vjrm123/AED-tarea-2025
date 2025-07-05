#include <iostream>
#include <stack>
#include <queue>
#include <utility> // Para std::pair
#include<mutex>
#include<thread>
int total = 0;
std::mutex mtx;
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


    void inorder(node* p);
    void preorder(node* p);
    void postorder(node* p);
    void inverse_inorder(node* p);
    void level_order();
    void reverse_level_order();
    void recolectarNodos(node* p, std::vector<node*>& nodos);
    void sumarrango(std::vector<node*>& nodos, int ini, int fin);
    long sumar();
    node* getRoot() { return root; }

private:
    node** Rep(node** p);
    node* root;
};

cbintree::cbintree() : root(nullptr) {}

cbintree::~cbintree() { clear(root); }

void cbintree::recolectarNodos(node* p, std::vector<node*>& nodos) {
    if (!p) return;
    nodos.push_back(p);
    recolectarNodos(p->nod[0], nodos);
    recolectarNodos(p->nod[1], nodos);
}

void cbintree::sumarrango(std::vector<node*>& nodos, int ini, int fin) {
    int parcial = 0;
    for (int i = ini; i < fin; i++) {
        parcial += nodos[i]->valor;
    }
    std::lock_guard<std::mutex> lock(mtx);
    total += parcial;
}

long cbintree::sumar() {
    std::vector<node*> nodos;
    recolectarNodos(root, nodos);

    total = 0;
    std::thread hilos[4];
    int n = nodos.size();
    int block = n / 4;
    int resto = n % 4;

    for (int i = 0; i < 4; i++) {
        int ini = i * block;
        int fin = ini + block + ((i < resto) ? 1 : 0);
        hilos[i] = std::thread(&cbintree::sumarrango,this, std::ref(nodos), ini, fin);
    }
    for (int i = 0; i < 4; i++) {
        hilos[i].join();
    }

    return total;
}


void cbintree::clear(node* p) {
    if (!p)return;
    clear(p->nod[0]);
    clear(p->nod[1]);
    delete p;
}

bool cbintree::find(int value, node**& p) {
    p = &root;
    while (*p && (*p)->valor != value) { p = &((*p)->nod[(*p)->valor < value]); }
    return (*p) != 0;
}

bool cbintree::add(int value) {
    node** p;
    if (find(value, p))return false;
    *p = new node(value);
    return true;
}

node** cbintree::Rep(node** p) {
    node** q = &((*p)->nod[1]);
    while ((*q)->nod[0]) { q = &((*q)->nod[0]); }
    return q;
}

bool cbintree::del(int value) {
    node** p;
    if (!find(value, p)) return false;
    if ((*p)->nod[1] && (*p)->nod[0]) {
        node** q = Rep(p);
        (*p)->valor = (*q)->valor;
        p = q;
    }
    node* t = *p;
    *p = (*p)->nod[(*p)->nod[1] != 0];
    delete t;
    return true;
}

void cbintree::inorder(node* p) {
    if (!p) return;
    inorder(p->nod[0]);
    cout << p->valor << " ";
    inorder(p->nod[1]);
}

void cbintree::preorder(node* p) {
    if (!p) return;
    cout << p->valor << " ";
    preorder(p->nod[0]);
    preorder(p->nod[1]);
}

void cbintree::postorder(node* p) {
    if (!p) return;
    postorder(p->nod[0]);
    postorder(p->nod[1]);
    cout << p->valor << " ";
}

void cbintree::inverse_inorder(node* p) {
    if (!p) return;
    inverse_inorder(p->nod[1]);
    cout << p->valor << " ";
    inverse_inorder(p->nod[0]);
}

void cbintree::level_order() {
    if (!root) return;
    queue<node*>q;
    q.push(root);
    while (!q.empty()) {
        node* current = q.front();
        q.pop();
        cout << current->valor << " ";
        if (current->nod[0]) q.push(current->nod[0]);
        if (current->nod[1]) q.push(current->nod[1]);
    }
}

void cbintree::reverse_level_order() {
    if (!root)return;
    queue<node*>q;
    stack<node*>p;
    q.push(root);
    while (!q.empty()) {
        node* current = q.front();
        q.pop();
        p.push(current);
        if (current->nod[1]) q.push(current->nod[1]);
        if (current->nod[0]) q.push(current->nod[0]);
    }
    while (!p.empty()) {
        cout << p.top()->valor << " ";
        p.pop();
    }
}


int main() {
    cbintree c;
    c.add(6); c.add(4); c.add(9); c.add(2); 
    long suma = c.sumar();
    std::cout << suma;
    return 0;
}