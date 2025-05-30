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

    
    void inorder(node* p);
    void preorder(node* p);
    void postorder(node* p);
    void inverse_inorder(node* p);
    void level_order();
    void reverse_level_order();
  
    node* getRoot() { return root; }

private:
    node** Rep(node** p);
    node* root;
};

cbintree::cbintree() : root(nullptr){}

cbintree::~cbintree() { clear(root); }

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
    c.add(6); c.add(4); c.add(9); c.add(2); c.add(7); c.add(8);
    c.add(1); c.add(6);
    cout << "inorder: ";
    c.inorder(c.getRoot());
    cout << "\n pre order: ";
    c.preorder(c.getRoot());
    cout << "\n post order: ";
    c.postorder(c.getRoot());

    cout << "\n inversa in order: ";
    c.inverse_inorder(c.getRoot());
    cout << "\n por niveles: ";
    c.level_order();
    cout << "\npor niveles inverso: ";
    c.reverse_level_order();


    return 0;
}