#include <iostream>
#include <stdexcept>  // Para std::out_of_range

class SparseMatrix {
private:
    struct Node {
        int row, col;
        double value;
        Node* right;  // Siguiente nodo en la fila
        Node* down;   // Siguiente nodo en la columna
        Node(int r, int c, double v) : row(r), col(c), value(v), right(nullptr), down(nullptr) {}
    };

    Node** rowHeads;  // Arreglo de punteros a los primeros nodos de cada fila
    Node** colHeads;  // Arreglo de punteros a los primeros nodos de cada columna
    int rows, cols;

public:
    // Constructor
    SparseMatrix(int rows, int cols) : rows(rows), cols(cols) {
        rowHeads = new Node*[rows]();  // Inicializa a nullptr
        colHeads = new Node*[cols]();
    }

    // Destructor
    ~SparseMatrix() {
        clear();
        delete[] rowHeads;
        delete[] colHeads;
    }

    // --- Métodos principales ---
    // Versión modificable (para asignación)
    double& operator()(int row, int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("Índices fuera de rango");
        }

        // Buscar el nodo existente o la posición para insertar
        Node* prevRow = nullptr;
        Node* currentRow = rowHeads[row];
        while (currentRow != nullptr && currentRow->col < col) {
            prevRow = currentRow;
            currentRow = currentRow->right;
        }

        // Si existe, retornar su valor
        if (currentRow != nullptr && currentRow->col == col) {
            return currentRow->value;
        }

        // Crear nuevo nodo con valor 0.0 (para poder modificarlo después)
        Node* newNode = new Node(row, col, 0.0);

        // Enlazar en la fila
        if (prevRow == nullptr) {
            newNode->right = rowHeads[row];
            rowHeads[row] = newNode;
        } else {
            newNode->right = prevRow->right;
            prevRow->right = newNode;
        }

        // Enlazar en la columna
        Node* prevCol = nullptr;
        Node* currentCol = colHeads[col];
        while (currentCol != nullptr && currentCol->row < row) {
            prevCol = currentCol;
            currentCol = currentCol->down;
        }

        if (prevCol == nullptr) {
            newNode->down = colHeads[col];
            colHeads[col] = newNode;
        } else {
            newNode->down = prevCol->down;
            prevCol->down = newNode;
        }

        return newNode->value;
    }

    // Versión constante (para lectura)
    const double& operator()(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("Índices fuera de rango");
        }

        Node* current = rowHeads[row];
        while (current != nullptr && current->col <= col) {
            if (current->col == col) {
                return current->value;
            }
            current = current->right;
        }

        static const double ZERO = 0.0;  // Retornar referencia a cero constante
        return ZERO;
    }

    // Método set (asignar valor)
    void set(int row, int col, double value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw std::out_of_range("Índices fuera de rango");
        }

        // Si el valor es cero, eliminar el nodo si existe
        if (value == 0.0) {
            remove(row, col);
            return;
        }

        // Usar el operador () para asignar
        (*this)(row, col) = value;
    }

    // Método get (obtener valor)
    double get(int row, int col) const {
        return (*this)(row, col);  // Reutiliza el operador () constante
    }

    // --- Métodos auxiliares ---
    // Eliminar un nodo
    void remove(int row, int col) {
        // Buscar en la fila
        Node* prevRow = nullptr;
        Node* currentRow = rowHeads[row];
        while (currentRow != nullptr && currentRow->col < col) {
            prevRow = currentRow;
            currentRow = currentRow->right;
        }

        if (currentRow == nullptr || currentRow->col != col) {
            return;  // Nodo no existe
        }

        // Desenlazar de la fila
        if (prevRow == nullptr) {
            rowHeads[row] = currentRow->right;
        } else {
            prevRow->right = currentRow->right;
        }

        // Buscar en la columna
        Node* prevCol = nullptr;
        Node* currentCol = colHeads[col];
        while (currentCol != nullptr && currentCol->row < row) {
            prevCol = currentCol;
            currentCol = currentCol->down;
        }

        // Desenlazar de la columna
        if (prevCol == nullptr) {
            colHeads[col] = currentCol->down;
        } else {
            prevCol->down = currentCol->down;
        }

        delete currentRow;
    }

    // Limpiar la matriz
    void clear() {
        for (int i = 0; i < rows; ++i) {
            Node* current = rowHeads[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->right;
                delete temp;
            }
            rowHeads[i] = nullptr;
        }
        for (int j = 0; j < cols; ++j) {
            colHeads[j] = nullptr;
        }
    }

    // Imprimir la matriz
    void print() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                std::cout << get(i, j) << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    SparseMatrix mat(3, 3);

    // Usar set para asignar valores
    mat.set(0, 0, 1.0);
    mat.set(1, 1, 2.0);
    mat.set(2, 2, 3.0);
    mat.set(0, 2, 4.0);

    // Usar get para leer valores
    std::cout << "Valor en (1,1): " << mat.get(1, 1) << "\n";

    // Usar operador () para asignar y leer
    mat(1, 0) = 5.0;
    std::cout << "Valor en (1,0): " << mat(1, 0) << "\n";

    std::cout << "Matriz completa:\n";
    mat.print();

    return 0;
}