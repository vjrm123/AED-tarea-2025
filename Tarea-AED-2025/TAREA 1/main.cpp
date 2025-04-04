#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

class MiVector {
private:
    vector<int> datos;

public:
    void push_back(int valor) { datos.push_back(valor); }
    int size() const { return datos.size(); }
    int& operator[](int index) { return datos[index]; }
    const int& operator[](int index) const { return datos[index]; }
};

void intercambiar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

//  directo
void bubbleSortDirecto(MiVector& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (vec[j] > vec[j + 1]) {
                intercambiar(vec[j], vec[j + 1]);
            }
        }
    }
}

// puntero función 
void bubbleSortFuncion(MiVector& vec, bool (*comparar)(int, int)) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (comparar(vec[j], vec[j + 1])) {
                intercambiar(vec[j], vec[j + 1]);
            }
        }
    }
}

bool ascendente(int a, int b) { return a > b; }

//functor
class BubbleSortFunctor {
public:
    void operator()(MiVector& vec) {
        int n = vec.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (vec[j] > vec[j + 1]) {
                    intercambiar(vec[j], vec[j + 1]);
                }
            }
        }
    }
};

//polimorfismo
class Ordenador {
public:
    virtual void ordenar(MiVector& vec) = 0;
    virtual ~Ordenador() {}
};

class BubbleSortPolimorfico : public Ordenador {
public:
    void ordenar(MiVector& vec) override {
        int n = vec.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (vec[j] > vec[j + 1]) {
                    intercambiar(vec[j], vec[j + 1]);
                }
            }
        }
    }
};

MiVector crearVectorDesordenado(int tamaño) {
    MiVector vec;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000);

    for (int i = 0; i < tamaño; ++i) {
        vec.push_back(dist(gen));
    }
    return vec;
}

void imprimirResultados() {
    vector<int> tamanios = { 500, 4000, 8000, 12000, 16000 };
    const int pruebas = 5;

    cout << "RESULTADOS \n";
    cout << "Tamanio\tDirecto\tFunciOn\tFunctor\tPolimorf.\n";
    cout << "----------------------------------------\n";

    for (int tam : tamanios) {
        MiVector original = crearVectorDesordenado(tam);
        BubbleSortFunctor functor;
        BubbleSortPolimorfico polimorfico;

        double t_directo = 0, t_funcion = 0, t_functor = 0, t_polimorfico = 0;

        for (int p = 0; p < pruebas; ++p) {
            MiVector copia = original;

            //Directo
            auto inicio = high_resolution_clock::now();
            bubbleSortDirecto(copia);
            auto fin = high_resolution_clock::now();
            t_directo += duration_cast<milliseconds>(fin - inicio).count();

            // Función
            copia = original;
            inicio = high_resolution_clock::now();
            bubbleSortFuncion(copia, ascendente);
            fin = high_resolution_clock::now();
            t_funcion += duration_cast<milliseconds>(fin - inicio).count();

            // Functor
            copia = original;
            inicio = high_resolution_clock::now();
            functor(copia);
            fin = high_resolution_clock::now();
            t_functor += duration_cast<milliseconds>(fin - inicio).count();

            // Polimorfismo
            copia = original;
            inicio = high_resolution_clock::now();
            polimorfico.ordenar(copia);
            fin = high_resolution_clock::now();
            t_polimorfico += duration_cast<milliseconds>(fin - inicio).count();
        }

        cout << tam << "\t"
            << t_directo / pruebas << "\t"
            << t_funcion / pruebas << "\t"
            << t_functor / pruebas << "\t"
            << t_polimorfico / pruebas << "\n";
    }
}

int main() {
    imprimirResultados();
    return 0;
}