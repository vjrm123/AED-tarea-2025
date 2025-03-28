#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric> 

class MiVector {
private:
    std::vector<int> datos;
    
public:
    void push_back(int valor) { datos.push_back(valor); }
    int size() const { return datos.size(); }
    int& operator[](int index) { return datos[index]; }
    const int& operator[](int index) const { return datos[index]; }
    void clear() { datos.clear(); }
};

void intercambiar(int& a, int& b) {
    std::swap(a, b);
}

// 1. Bubble Sort directo (más eficiente)
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

// 2. Bubble Sort con functor
class BubbleSortFunctor {
public:
    void operator()(MiVector& vec) const {
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

// 3. Bubble Sort con puntero a función
void bubbleSortFuncion(MiVector& vec, bool (*cmp)(int, int)) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (cmp(vec[j + 1], vec[j])) {
                intercambiar(vec[j], vec[j + 1]);
            }
        }
    }
}

bool ascendente(int a, int b) { return a < b; }

// 4. Bubble Sort con polimorfismo (menos eficiente)
class Ordenador {
public:
    virtual void ordenar(MiVector& vec) = 0;
    virtual ~Ordenador() = default;
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
    unsigned int semilla = 12345;
    for (int i = 0; i < tamaño; ++i) {
        semilla = (8253729 * semilla + 2396403);
        vec.push_back(semilla % 1000);
    }
    return vec;
}

void ejecutarPruebas(int tamaño) {
    std::cout << "\nTamaño: " << tamaño << " elementos\n";
    std::cout << "----------------------------------\n";
    
    MiVector original = crearVectorDesordenado(tamaño);
    BubbleSortFunctor functor;
    BubbleSortPolimorfico polimorfico;
    
    // Almacenar tiempos [directo, functor, funcion, polimorfico]
    std::vector<std::vector<long>> tiempos(4);
    
    for (int prueba = 0; prueba < 5; ++prueba) {
        // 1. Directo
        MiVector copia = original;
        auto inicio = std::chrono::high_resolution_clock::now();
        bubbleSortDirecto(copia);
        auto fin = std::chrono::high_resolution_clock::now();
        tiempos[0].push_back(std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count());
        
        // 2. Functor
        copia = original;
        inicio = std::chrono::high_resolution_clock::now();
        functor(copia);
        fin = std::chrono::high_resolution_clock::now();
        tiempos[1].push_back(std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count());
        
        // 3. Función
        copia = original;
        inicio = std::chrono::high_resolution_clock::now();
        bubbleSortFuncion(copia, ascendente);
        fin = std::chrono::high_resolution_clock::now();
        tiempos[2].push_back(std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count());
        
        // 4. Polimorfismo
        copia = original;
        inicio = std::chrono::high_resolution_clock::now();
        polimorfico.ordenar(copia);
        fin = std::chrono::high_resolution_clock::now();
        tiempos[3].push_back(std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count());
    }
    
    // Calcular promedios
    std::vector<double> promedios = {
        std::accumulate(tiempos[0].begin(), tiempos[0].end(), 0.0) / 5,
        std::accumulate(tiempos[1].begin(), tiempos[1].end(), 0.0) / 5,
        std::accumulate(tiempos[2].begin(), tiempos[2].end(), 0.0) / 5,
        std::accumulate(tiempos[3].begin(), tiempos[3].end(), 0.0) / 5
    };
    
    // Mostrar resultados
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "1. Directo:       " << promedios[0] << " ms\n";
    std::cout << "2. Functor:      " << promedios[1] << " ms\n";
    std::cout << "3. Puntero Fun.: " << promedios[2] << " ms\n";
    std::cout << "4. Polimorfismo: " << promedios[3] << " ms\n";
}

int main() {
    std::vector<int> tamanios = {5000, 10000, 15000, 20000, 25000};
    
    for (int tamaño : tamanios) {
        ejecutarPruebas(tamaño);
    }
    
    return 0;
}