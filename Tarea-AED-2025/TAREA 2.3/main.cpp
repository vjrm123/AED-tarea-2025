#include <iostream>
#include <SFML/Graphics.hpp>
#include <queue>
#include <map>
using namespace std;

struct arbol_nodo {
    int val;
    arbol_nodo* arr[2];
    arbol_nodo(int v) {
        val = v;
        arr[0] = NULL;
        arr[1] = NULL;
    }
};

class arbol {
private:
    bool find(int v, arbol_nodo**& x);
    arbol_nodo* root;
    arbol_nodo** recorrer(arbol_nodo**& y);
    void in_order(arbol_nodo* n);
    void dibujar(sf::RenderWindow& window, arbol_nodo* node, float x, float y, float offsetX, float offsetY);

    
    std::map<arbol_nodo*, sf::Color> colores;

public:
    bool push(int v);
    bool remove(int v);
    void print();
    void pintar_tios_primos(int valor);
    void VecInsert(arbol* t, int* v, int size);

    arbol();
};

arbol::arbol() {
    root = NULL;
}

bool arbol::find(int v, arbol_nodo**& x) {
    x = &root;
    while (*x && (*x)->val != v) {
        x = &((*x)->arr[v > (*x)->val]);
    }
    return *x != 0;
}

arbol_nodo** arbol::recorrer(arbol_nodo**& z) {
    arbol_nodo** y;
    y = &((*z)->arr[0]);
    while ((*y)->arr[1] != NULL) {
        y = &((*y)->arr[1]);
    }
    return y;
}

bool arbol::push(int v) {
    arbol_nodo** b;
    if (find(v, b)) {
        return 0;
    }
    *b = new arbol_nodo(v);
    return 1;
}

bool arbol::remove(int v) {
    arbol_nodo** b;
    if (!find(v, b)) {
        return 0;
    }
    else {
        if ((*b)->arr[0] && (*b)->arr[1]) {
            arbol_nodo** y = recorrer(b);
            (*b)->val = (*y)->val;
            b = y;
        }
        arbol_nodo* h = *b;
        *b = (*b)->arr[(*b)->arr[0] == 0];
        delete h;
        return 1;
    }
}


void arbol::pintar_tios_primos(int valor) {
    if (!root) return;

    std::queue<std::pair<arbol_nodo*, int>> q;
    q.push({ root, 0 });

    arbol_nodo* nodo_objetivo = nullptr;
    int nivel_objetivo = -1;
    arbol_nodo* padre_objetivo = nullptr;

    
    while (!q.empty()) {
        arbol_nodo* nodo = q.front().first;
        int nivel = q.front().second;
        q.pop();

        if ((nodo->arr[0] && nodo->arr[0]->val == valor) ||
            (nodo->arr[1] && nodo->arr[1]->val == valor)) {
            nodo_objetivo = (nodo->arr[0] && nodo->arr[0]->val == valor) ? nodo->arr[0] : nodo->arr[1];
            nivel_objetivo = nivel + 1;
            padre_objetivo = nodo;
            break;
        }

        if (nodo->arr[0]) q.push({ nodo->arr[0], nivel + 1 });
        if (nodo->arr[1]) q.push({ nodo->arr[1], nivel + 1 });
    }

    if (!nodo_objetivo) return; 


    q.push({ root, 0 });
    while (!q.empty()) {
        arbol_nodo* nodo = q.front().first;
        int nivel = q.front().second;
        q.pop();


        if (nivel == nivel_objetivo && nodo != nodo_objetivo) {
            if (padre_objetivo && (padre_objetivo->arr[0] != nodo && padre_objetivo->arr[1] != nodo)) {
                colores[nodo] = sf::Color::Blue; 
            }
        }


        if (nivel == nivel_objetivo - 1 && nodo != padre_objetivo) {
            colores[nodo] = sf::Color::Green; 
        }

        if (nodo->arr[0]) q.push({ nodo->arr[0], nivel + 1 });
        if (nodo->arr[1]) q.push({ nodo->arr[1], nivel + 1 });
    }
}

void arbol::dibujar(sf::RenderWindow& window, arbol_nodo* node, float x, float y, float offsetX, float offsetY) {
    if (node != nullptr) {
        sf::CircleShape circle(25);

        
        if (colores.find(node) != colores.end()) {
            circle.setFillColor(colores[node]);
        }
        else {
            circle.setFillColor(sf::Color::Red);
        }

        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(2);
        circle.setPosition(x, y);
        window.draw(circle);

        sf::Font font;
        font.loadFromFile("C:/WINDOWS/FONTS/arial.ttf");
        sf::Text text(std::to_string(node->val), font, 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 10, y + 10);
        window.draw(text);

        if (node->arr[0] != nullptr) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x + 25, y + 25)),
                sf::Vertex(sf::Vector2f(x - offsetX + 25, y + offsetY + 25))
            };
            window.draw(line, 2, sf::Lines);
            dibujar(window, node->arr[0], x - offsetX, y + offsetY, offsetX * 0.5f, offsetY * 0.9f);
        }
        if (node->arr[1] != nullptr) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x + 25, y + 25)),
                sf::Vertex(sf::Vector2f(x + offsetX + 25, y + offsetY + 25))
            };
            window.draw(line, 2, sf::Lines);
            dibujar(window, node->arr[1], x + offsetX, y + offsetY, offsetX * 0.5f, offsetY * 0.9f);
        }
    }
}

void arbol::print() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Arbol");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        if (root != nullptr)
            dibujar(window, root, 400, 50, 200, 100);
        window.display();
    }
}

void arbol::VecInsert(arbol* t, int* v, int size) {
    if (size <= 0) { return; }
    t->push(v[size / 2]);
    VecInsert(t, v, size / 2);
    VecInsert(t, v + size / 2 + 1, size - size / 2 - 1);
}

int main() {
    arbol h;
    int numbers[30];
    for (int i = 0; i < 30; i++) {
        numbers[i] = i + 1;
    }

    h.VecInsert(&h, numbers, 30);

    h.pintar_tios_primos(1);
    h.print();

    return 0;
}