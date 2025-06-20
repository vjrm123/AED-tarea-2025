#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Nodo {
    int valor;
    Nodo* hijos[2];

    Nodo(int val) : valor(val) {
        hijos[0] = hijos[1] = nullptr;
    }
};

class ArbolVisual {
private:
    Nodo* raiz;
    Font fuente;
    mutex mtx;  

    const Color COLOR_NODO = Color(70, 130, 180);
    const Color COLOR_BORDE = Color(200, 200, 200);
    const Color COLOR_LINEA = Color(180, 180, 180);
    const Color COLOR_TEXTO = Color::White;
    const Color COLOR_FONDO = Color(15, 15, 40);

    const float RADIO_NODO = 22.f;
    const float GROSOR_BORDE = 1.5f;
    const int TAMANO_TEXTO = 20;
    const int ESPACIADO_VERTICAL = 80;
    const float FACTOR_REDUCCION = 0.55f;

    void limpiar(Nodo* nodo) {
        if (!nodo) return;
        limpiar(nodo->hijos[0]);
        limpiar(nodo->hijos[1]);
        delete nodo;
    }

    float calcularEspaciado(int profundidad) {
        return 200.f * pow(FACTOR_REDUCCION, profundidad);
    }

    void dibujarNodo(RenderWindow& ventana, Nodo* nodo, float x, float y) {
        CircleShape circulo(RADIO_NODO);
        circulo.setFillColor(COLOR_NODO);
        circulo.setOutlineThickness(GROSOR_BORDE);
        circulo.setOutlineColor(COLOR_BORDE);
        circulo.setPosition(x - RADIO_NODO, y - RADIO_NODO);
        ventana.draw(circulo);

        Text texto(to_string(nodo->valor), fuente, TAMANO_TEXTO);
        texto.setFillColor(COLOR_TEXTO);
        texto.setStyle(Text::Bold);

        FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin(bounds.width / 2, bounds.height / 2);
        texto.setPosition(x, y);
        ventana.draw(texto);
    }

    void dibujarConexiones(RenderWindow& ventana, Nodo* nodo, float x, float y, float espaciado, int profundidad) {
        for (int i = 0; i < 2; i++) {
            if (nodo->hijos[i]) {
                float hijoX = x + (i == 0 ? -espaciado : espaciado);
                float hijoY = y + ESPACIADO_VERTICAL;

                Vertex linea[] = {
                    Vertex(Vector2f(x, y + RADIO_NODO), COLOR_LINEA),
                    Vertex(Vector2f(hijoX, hijoY - RADIO_NODO), COLOR_LINEA)
                };
                ventana.draw(linea, 2, Lines);

                dibujarArbol(ventana, nodo->hijos[i], hijoX, hijoY, profundidad + 1);
            }
        }
    }

    void dibujarArbol(RenderWindow& ventana, Nodo* nodo, float x, float y, int profundidad) {
        if (!nodo) return;

        float espaciado = calcularEspaciado(profundidad);
        dibujarConexiones(ventana, nodo, x, y, espaciado, profundidad);
        dibujarNodo(ventana, nodo, x, y);
    }

    void construirSubarboles(vector<int>& hojas, Nodo*& subRaiz) {
        queue<Nodo*> nivelActual;
        for (int hoja : hojas) {
            nivelActual.push(new Nodo(hoja));
        }

        while (nivelActual.size() > 1) {
            queue<Nodo*> siguienteNivel;

            while (!nivelActual.empty()) {
                Nodo* izquierdo = nivelActual.front();
                nivelActual.pop();

                Nodo* derecho = nullptr;
                if (!nivelActual.empty()) {
                    derecho = nivelActual.front();
                    nivelActual.pop();
                }

                Nodo* padre = new Nodo(izquierdo->valor + (derecho ? derecho->valor : 0));
                padre->hijos[0] = izquierdo;
                padre->hijos[1] = derecho;
                siguienteNivel.push(padre);
            }

            nivelActual = siguienteNivel;
        }

        subRaiz = nivelActual.front();
    }

    void combinarArboles(Nodo* subRaiz1, Nodo* subRaiz2, Nodo*& raiz) {
        Nodo* padre = new Nodo(subRaiz1->valor + subRaiz2->valor);
        padre->hijos[0] = subRaiz1;
        padre->hijos[1] = subRaiz2;

        
        lock_guard<mutex> lock(mtx);
        raiz = padre;
    }

public:
    ArbolVisual() : raiz(nullptr) {
        if (!fuente.loadFromFile("Mayan.ttf")) {
            cerr << "Error cargando fuente\n";
        }
    }

    ~ArbolVisual() {
        limpiar(raiz);
    }

    void construirDesdeHojas(const vector<int>& hojas, int numThreads) {
        limpiar(raiz);
        if (hojas.empty()) return;

        
        vector<vector<int>> partes(numThreads);
        int tamanoParte = hojas.size() / numThreads;
        int sobra = hojas.size() % numThreads;

        for (int i = 0; i < numThreads; ++i) {
            int inicio = i * tamanoParte;
            int fin = inicio + tamanoParte;
            if (i == numThreads - 1) {
                fin += sobra;  
            }
            partes[i] = vector<int>(hojas.begin() + inicio, hojas.begin() + fin);
        }

        vector<Nodo*> subArboles(numThreads);
        vector<thread> threads;

        
        for (int i = 0; i < numThreads; ++i) {
            threads.push_back(thread(&ArbolVisual::construirSubarboles, this, ref(partes[i]), ref(subArboles[i])));
        }

        for (auto& th : threads) {
            th.join();
        }

        
        while (subArboles.size() > 1) {
            vector<Nodo*> nuevosSubArboles;

            for (size_t i = 0; i < subArboles.size(); i += 2) {
                if (i + 1 < subArboles.size()) {
                    Nodo* subArbol1 = subArboles[i];
                    Nodo* subArbol2 = subArboles[i + 1];
                    Nodo* padre = new Nodo(subArbol1->valor + subArbol2->valor);
                    padre->hijos[0] = subArbol1;
                    padre->hijos[1] = subArbol2;
                    nuevosSubArboles.push_back(padre);
                }
                else {
                    nuevosSubArboles.push_back(subArboles[i]);
                }
            }

            subArboles = nuevosSubArboles;
        }

        raiz = subArboles.front();  
    }

    void visualizar() {
        RenderWindow ventana(VideoMode(1000, 700), "Árbol Binario con Hilos");
        ventana.setFramerateLimit(60);

        while (ventana.isOpen()) {
            Event evento;
            while (ventana.pollEvent(evento)) {
                if (evento.type == Event::Closed)
                    ventana.close();
            }

            ventana.clear(COLOR_FONDO);

            if (raiz) {
                dibujarArbol(ventana, raiz, 500, 120, 0);
            }

            ventana.display();
        }
    }
};

int main() {
    ArbolVisual arbol;
    vector<int> hojas = { 3, 5, 2, 7, 1, 4, 6, 5, 8, 9, 10, 11 };

    int numThreads = 4;  
    arbol.construirDesdeHojas(hojas, numThreads);
    arbol.visualizar();

    return 0;
}