#include <iostream>
#include<vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CBinNode
{
    CBinNode(int _v)
    {
        value = _v; nodes[0] = nodes[1] = 0;
    }
    int value;
    CBinNode* nodes[2];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

class CBinTree
{
public:
    CBinTree();
    ~CBinTree();
    bool Insert(int x);
    void Print();
    void kneighbors(int x, int k);
    void Rango(CBinNode* n, int a, int b);
    void mostrarEnRango(int a, int b);
    void imprimirAncestros(int x);
    void TiosYPrimos(int x);
private:
    bool Find(int x, CBinNode**& p);
    void InOrder(CBinNode* n);
    CBinNode* m_root;
};

CBinTree::CBinTree()
{
    m_root = 0;
}

CBinTree::~CBinTree()
{
}

bool CBinTree::Find(int x, CBinNode**& p)
{
    for (p = &m_root; *p && (*p)->value != x; p = &((*p)->nodes[(*p)->value < x]));
    return *p && (*p)->value == x;
}

bool CBinTree::Insert(int x)
{
    CBinNode** p;
    if (Find(x, p)) return 0;
    *p = new CBinNode(x);
    return 0;
}

void CBinTree::InOrder(CBinNode* n)
{
    if (!n) return;
    InOrder(n->nodes[0]);
    cout << n->value << " ";
    InOrder(n->nodes[1]);
}

void CBinTree::Print()
{
    InOrder(m_root);
    cout << endl;
}

void CBinTree::kneighbors(int x, int k) {
    std::cout << "\n(" << x << "," << k << ") => ";
    CBinNode** p;
    if (Find(x, p)) {
        cout << x << " ";
        k--;
    }
    int men = x - 1;
    int may = x + 1;
    while (k > 0) {
        bool foundmen = Find(men, p);
        bool foundmay = Find(may, p);
        if (foundmen && foundmay) {
            if (abs(men - x) <= (may - x)) {
                cout << men << " ";
                men--;
            }
            else {
                cout << may << " ";
                may++;
            }
        }
        else if (foundmen) {
            cout << men << " ";
            men--;
        }
        else if(foundmay){
            cout << may << " ";
            may++;
        }
        else {
            men--;
            may++;
            continue;
        }
        k--;
    }
    
}

void CBinTree::Rango(CBinNode* n, int a, int b) {
    if (!n) return;
    if (n->value > a) Rango(n->nodes[0], a, b);
    if (n->value >= a && n->value <= b) cout << n->value << " ";
    if (n->value < b) Rango(n->nodes[1], a, b);
}

void CBinTree::mostrarEnRango(int a, int b) {
    Rango(m_root, a, b);
    cout << endl;
}

void CBinTree::imprimirAncestros(int x) {
    CBinNode* p = m_root;
    cout << "Ancestros de " << x << ": ";
    while (p && p->value != x) {
        cout << p->value << " ";
        p = p->nodes[p->value < x];
    }
    if (!p) cout << "(no encontrado)";
    cout << endl;
}

void CBinTree::TiosYPrimos(int x) {
    CBinNode** p;
    if (!Find(x, p)) {
        cout << "No existe el nodo " << x << endl;
        return;
    }

    CBinNode* padre = nullptr, * abuelo = nullptr, * actual = m_root;
    while (actual && actual->value != x) {
        abuelo = padre;
        padre = actual;
        actual = actual->nodes[actual->value < x];
    }

    cout << "Tío(s) de " << x << ": ";
    if (!abuelo) {
        cout << "Ninguno (no tiene abuelo)" << endl;
    }
    else {
        CBinNode* tio = (abuelo->nodes[0] == padre) ? abuelo->nodes[1] : abuelo->nodes[0];

        if (tio) {
            cout << tio->value << endl << "Primo(s) de " << x << ": ";
            if (tio->nodes[0]) cout << tio->nodes[0]->value << " ";
            if (tio->nodes[1]) cout << tio->nodes[1]->value << " ";
        }
        else {
            cout << "Ninguno (padre es hijo único)" << endl
                << "Primo(s) de " << x << ": Ninguno";
        }
    }
    cout << endl;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    CBinTree t;
    t.Insert(55); t.Insert(41); t.Insert(77);
    t.Insert(33); t.Insert(47); t.Insert(61);
    t.Insert(88); t.Insert(20); t.Insert(36);
    t.Insert(44); t.Insert(51); t.Insert(57);
    t.Insert(65); t.Insert(80); t.Insert(99);
    t.Print();

    t.kneighbors(33, 4);
    t.kneighbors(88, 3);
    t.kneighbors(76, 2);
    t.kneighbors(47, 5);
    t.kneighbors(61, 4);
    t.kneighbors(50, 3);
    t.kneighbors(81, 5);
    t.kneighbors(20, 7);
    cout << "\nValores entre 40 y 60: ";
    t.mostrarEnRango(40, 60);
    t.imprimirAncestros(44);

    t.TiosYPrimos(36);

    cout << endl;
}