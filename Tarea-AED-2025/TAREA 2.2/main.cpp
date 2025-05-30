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

class arbol_binario {
public:
    arbol_binario() : raiz(nullptr) {}
    ~arbol_binario() { limpiar(raiz); }

    bool agregar(int valor);
    void dibujar(sf::RenderWindow& ventana, sf::Font& fuente, sf::Vector2i posicionMouse);
    nodo* obtenerRaiz() { return raiz; }

private:
    nodo* raiz;

    void limpiar(nodo* p);
    void calcularPosiciones(nodo* p, int x, int y, int desplazamientoHorizontal, map<nodo*, pair<int, int>>& posiciones);
    void encontrarExtremos(nodo* p, int profundidad, map<int, nodo*>& masIzquierdo, map<int, nodo*>& masDerecho);
    void dibujarNodoConColor(sf::RenderWindow& ventana, nodo* nodoPtr, sf::Font& fuente, int xPos, int yPos, bool esContorno);
    void dibujarConexiones(sf::RenderWindow& ventana, nodo* nodoPtr, const map<nodo*, pair<int, int>>& posiciones);
    void dibujarArbol(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones);
};

bool arbol_binario::agregar(int valor) {
    nodo** p = &raiz;
    while (*p != nullptr) {
        if ((*p)->valor == valor) return false;
        p = &((*p)->hijos[(*p)->valor < valor]);
    }
    *p = new nodo(valor);
    return true;
}

void arbol_binario::limpiar(nodo* p) {
    if (!p) return;
    limpiar(p->hijos[0]);
    limpiar(p->hijos[1]);
    delete p;
}

void arbol_binario::calcularPosiciones(nodo* p, int x, int y, int desplazamientoHorizontal, map<nodo*, pair<int, int>>& posiciones) {
    if (!p) return;
    posiciones[p] = make_pair(x, y);
    calcularPosiciones(p->hijos[0], x - desplazamientoHorizontal, y + 80, desplazamientoHorizontal / 2, posiciones);
    calcularPosiciones(p->hijos[1], x + desplazamientoHorizontal, y + 80, desplazamientoHorizontal / 2, posiciones);
}

void arbol_binario::encontrarExtremos(nodo* p, int profundidad, map<int, nodo*>& masIzquierdo, map<int, nodo*>& masDerecho) {
    if (!p) return;

    if (masIzquierdo.find(profundidad) == masIzquierdo.end()) {
        masIzquierdo[profundidad] = p;
    }

    masDerecho[profundidad] = p;

    encontrarExtremos(p->hijos[0], profundidad + 1, masIzquierdo, masDerecho);
    encontrarExtremos(p->hijos[1], profundidad + 1, masIzquierdo, masDerecho);
}

void arbol_binario::dibujarNodoConColor(sf::RenderWindow& ventana, nodo* nodoPtr, sf::Font& fuente, int xPos, int yPos, bool esContorno) {
    sf::CircleShape circulo(20);

    if (esContorno) {
        circulo.setFillColor(sf::Color::Yellow);
    }
    else {
        circulo.setFillColor(sf::Color::Green);
    }

    circulo.setPosition(xPos, yPos);
    ventana.draw(circulo);

    sf::Text texto;
    texto.setFont(fuente);
    texto.setString(to_string(nodoPtr->valor));
    texto.setCharacterSize(25);
    texto.setFillColor(sf::Color::Black);

    sf::FloatRect rectTexto = texto.getLocalBounds();
    texto.setOrigin(rectTexto.left + rectTexto.width / 2.0f, rectTexto.top + rectTexto.height / 2.0f);
    texto.setPosition(xPos + 20, yPos + 20);
    ventana.draw(texto);
}

void arbol_binario::dibujarConexiones(sf::RenderWindow& ventana, nodo* nodoPtr, const map<nodo*, pair<int, int>>& posiciones) {
    if (nodoPtr->hijos[0] != nullptr) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(posiciones.at(nodoPtr).first + 20, posiciones.at(nodoPtr).second + 20)),
            sf::Vertex(sf::Vector2f(posiciones.at(nodoPtr->hijos[0]).first + 20, posiciones.at(nodoPtr->hijos[0]).second + 20))
        };
        ventana.draw(linea, 2, sf::Lines);
    }
    if (nodoPtr->hijos[1] != nullptr) {
        sf::Vertex linea[] = {
            sf::Vertex(sf::Vector2f(posiciones.at(nodoPtr).first + 20, posiciones.at(nodoPtr).second + 20)),
            sf::Vertex(sf::Vector2f(posiciones.at(nodoPtr->hijos[1]).first + 20, posiciones.at(nodoPtr->hijos[1]).second + 20))
        };
        ventana.draw(linea, 2, sf::Lines);
    }
}

void arbol_binario::dibujarArbol(sf::RenderWindow& ventana, nodo* p, sf::Font& fuente, map<nodo*, pair<int, int>>& posiciones) {
    map<int, nodo*> masIzquierdo;
    map<int, nodo*> masDerecho;

    encontrarExtremos(p, 0, masIzquierdo, masDerecho);

    for (auto& it : posiciones) {
        dibujarConexiones(ventana, it.first, posiciones);
    }

    for (auto& it : posiciones) {
        nodo* nodoActual = it.first;
        int xPos = it.second.first;
        int yPos = it.second.second;

        bool esContorno = (masIzquierdo[it.second.second / 80] == nodoActual ||
            masDerecho[it.second.second / 80] == nodoActual ||
            (nodoActual->hijos[0] == nullptr && nodoActual->hijos[1] == nullptr));

        dibujarNodoConColor(ventana, nodoActual, fuente, xPos, yPos, esContorno);
    }
}

void arbol_binario::dibujar(sf::RenderWindow& ventana, sf::Font& fuente, sf::Vector2i posicionMouse) {
    if (raiz == nullptr) return;

    map<nodo*, pair<int, int>> posiciones;
    calcularPosiciones(raiz, 400, 50, 200, posiciones);
    dibujarArbol(ventana, raiz, fuente, posiciones);
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(900, 600), "arbol Binario");
    sf::Font fuente;

    if (!fuente.loadFromFile("Mayan.ttf")) {
        cerr << "Error cargando la fuente" << endl;
        return -1;
    }

    arbol_binario arbol;

    arbol.agregar(50);

    arbol.agregar(25); arbol.agregar(75);

    arbol.agregar(10); arbol.agregar(30);
    arbol.agregar(60); arbol.agregar(90);

    arbol.agregar(5); arbol.agregar(15); arbol.agregar(28); arbol.agregar(35);
    arbol.agregar(55); arbol.agregar(65); arbol.agregar(80); arbol.agregar(95);

    arbol.agregar(2); arbol.agregar(7); arbol.agregar(12); arbol.agregar(18); arbol.agregar(26);
    arbol.agregar(29); arbol.agregar(32); arbol.agregar(40); arbol.agregar(52); arbol.agregar(57);
    arbol.agregar(62); arbol.agregar(68); arbol.agregar(78); arbol.agregar(85); arbol.agregar(92);
    arbol.agregar(98);

    arbol.agregar(17); arbol.agregar(38); arbol.agregar(54);
    arbol.agregar(8); arbol.agregar(100); arbol.agregar(94);
    arbol.agregar(27); arbol.agregar(69); arbol.agregar(63);



    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear(sf::Color(173, 216, 230));
        sf::Vector2i posicionMouse = sf::Mouse::getPosition(ventana);

        arbol.dibujar(ventana, fuente, posicionMouse);

        ventana.display();
    }

    return 0;
}