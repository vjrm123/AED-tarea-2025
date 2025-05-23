#include <SFML/Graphics.hpp> 
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct nodo {
    int valor;
    nodo* hijos[2];

    nodo(int valor) {
        this->valor = valor;
        hijos[0] = hijos[1] = nullptr;
    }
};

class arbolBinario {
public:
    arbolBinario() : raiz(nullptr) {}
    ~arbolBinario() { limpiar(raiz); }

    bool encontrar(int valor, nodo**& p);
    bool agregar(int valor);
    bool eliminar(int valor);
    void dibujar(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, sf::Vector2i posicionRaton);
    nodo* obtenerRaiz() { return raiz; }

private:
    nodo* raiz;
    void limpiar(nodo* p);
    nodo** reemplazo(nodo** p);
    void calcularPosiciones(nodo* p, int x, int y, int desplazamientoHorizontal, map<nodo*, pair<int, int>>& posiciones, int& profundidad);
    void dibujarNodo(sf::RenderWindow& ventana, nodo* nodoActual, sf::Font& fuente, int xPos, int yPos, sf::Vector2i posicionRaton);
    void dibujarConexiones(sf::RenderWindow& ventana, nodo* nodoActual, const map<nodo*, pair<int, int>>& posiciones);
    void dibujarInOrden(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones);
    void dibujarPreOrden(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones);
};

void arbolBinario::limpiar(nodo* p) {
    if (!p) return;
    limpiar(p->hijos[0]);
    limpiar(p->hijos[1]);
    delete p;
}

bool arbolBinario::encontrar(int valor, nodo**& p) {
    p = &raiz;
    while (*p && (*p)->valor != valor) {
        p = &((*p)->hijos[(*p)->valor < valor]);
    }
    return *p != nullptr;
}

bool arbolBinario::agregar(int valor) {
    nodo** p;
    if (encontrar(valor, p)) return false;
    *p = new nodo(valor);
    return true;
}

nodo** arbolBinario::reemplazo(nodo** p) {
    nodo** q = &((*p)->hijos[1]);
    while ((*q)->hijos[0]) {
        q = &((*q)->hijos[0]);
    }
    return q;
}

bool arbolBinario::eliminar(int valor) {
    nodo** p;
    if (!encontrar(valor, p)) return false;
    if ((*p)->hijos[0] && (*p)->hijos[1]) {
        nodo** q = reemplazo(p);
        (*p)->valor = (*q)->valor;
        p = q;
    }
    nodo* t = *p;
    *p = (*p)->hijos[(*p)->hijos[1] != nullptr];
    delete t;
    return true;
}

void arbolBinario::calcularPosiciones(nodo* p, int x, int y, int desplazamientoHorizontal, map<nodo*, pair<int, int>>& posiciones, int& profundidad) {
    if (!p) return;
    posiciones[p] = make_pair(x, profundidad++);
    calcularPosiciones(p->hijos[0], x - desplazamientoHorizontal, y + 80, desplazamientoHorizontal / 2, posiciones, profundidad);
    calcularPosiciones(p->hijos[1], x + desplazamientoHorizontal, y + 80, desplazamientoHorizontal / 2, posiciones, profundidad);
}

void arbolBinario::dibujarNodo(sf::RenderWindow& ventana, nodo* nodoActual, sf::Font& fuente, int xPos, int yPos, sf::Vector2i posicionRaton) {
    sf::CircleShape circulo(20);
    circulo.setFillColor(sf::Color(70, 130, 180)); 
    circulo.setOutlineThickness(1);
    circulo.setOutlineColor(sf::Color(200, 200, 200)); 

    circulo.setPosition(xPos, yPos);
    ventana.draw(circulo);

    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(to_string(nodoActual->valor));
    texto.setCharacterSize(25);
    texto.setFillColor(sf::Color::White); 

    sf::FloatRect rectTexto = texto.getLocalBounds();
    texto.setOrigin(rectTexto.left + rectTexto.width / 2.0f, rectTexto.top + rectTexto.height / 2.0f);
    texto.setPosition(xPos + 20, yPos + 20);
    ventana.draw(texto);

}

void arbolBinario::dibujarConexiones(sf::RenderWindow& ventana, nodo* nodoActual, const map<nodo*, pair<int, int>>& posiciones) {
    sf::Color colorLinea(180, 180, 180);

    if (nodoActual->hijos[0] != nullptr) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(posiciones.at(nodoActual).first + 20, posiciones.at(nodoActual).second * 80 + 50 + 20), colorLinea),
            sf::Vertex(sf::Vector2f(posiciones.at(nodoActual->hijos[0]).first + 20, posiciones.at(nodoActual->hijos[0]).second * 80 + 50 + 20), colorLinea)
        };
        ventana.draw(linea, 2, sf::Lines);
    }

    if (nodoActual->hijos[1] != nullptr) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(posiciones.at(nodoActual).first + 20, posiciones.at(nodoActual).second * 80 + 50 + 20), colorLinea),
            sf::Vertex(sf::Vector2f(posiciones.at(nodoActual->hijos[1]).first + 20, posiciones.at(nodoActual->hijos[1]).second * 80 + 50 + 20), colorLinea)
        };
        ventana.draw(linea, 2, sf::Lines);
    }
}


void arbolBinario::dibujarInOrden(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones) {
    if (p == nullptr) return;
    dibujarInOrden(ventana, p->hijos[0], fuente, posiciones);

    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(to_string(p->valor));
    texto.setCharacterSize(28);
    texto.setFillColor(sf::Color(180, 180, 180));
    texto.setPosition(posiciones[p].first + 5, 2);
    ventana.draw(texto);

    dibujarInOrden(ventana, p->hijos[1], fuente, posiciones);
}

void arbolBinario::dibujarPreOrden(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones) {
    if (p == nullptr) return;

    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(to_string(p->valor));
    texto.setCharacterSize(28);
    texto.setFillColor(sf::Color(180, 180, 180));
    texto.setPosition(50, posiciones[p].second * 80 + 50);
    ventana.draw(texto);

    dibujarPreOrden(ventana, p->hijos[0], fuente, posiciones);
    dibujarPreOrden(ventana, p->hijos[1], fuente, posiciones);
}

void arbolBinario::dibujar(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, sf::Vector2i posicionRaton) {
    if (p == nullptr) return;

    map<nodo*, pair<int, int>> posiciones;
    int profundidad = 0; 
    calcularPosiciones(p, 400, 50, 200, posiciones, profundidad);

    dibujarPreOrden(ventana, p, fuente, posiciones);
    dibujarInOrden(ventana, p, fuente, posiciones);

    for (auto& it : posiciones) {
        dibujarConexiones(ventana, it.first, posiciones);
        dibujarNodo(ventana, it.first, fuente, it.second.first, it.second.second * 80 + 50, posicionRaton);
    }
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Árbol Binario");
    sf::Font fuente;

    if (!fuente.loadFromFile("Mayan.ttf")) {
        std::cerr << "Error cargando la fuente" << std::endl;
        return -1;
    }

    arbolBinario arbol;
    arbol.agregar(5);
    arbol.agregar(2);
    arbol.agregar(8);
    arbol.agregar(1);
    arbol.agregar(3);
    arbol.agregar(6);
    arbol.agregar(10);

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed) {
                ventana.close();
            }
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(ventana);
        ventana.clear(sf::Color(15, 15, 40));
        arbol.dibujar(ventana, arbol.obtenerRaiz(), fuente, mousePos);
        ventana.display();
    }

    return 0;
}
