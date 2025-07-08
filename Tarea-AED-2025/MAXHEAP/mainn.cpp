
#include <iostream>
#include <deque>
#include<vector>

template <class T>
class CMaxHeap3
{
public:
    void push(int n);
    int top();
    void pop();
    void print();
private:
    std::deque<T> heap;
    void heapifyup(int i);
    void heapfyDown(int i);
};

template <class T>
int CMaxHeap3<T>::top()
{
    if (!heap.empty()) {
        return heap[0];
    }
    std::cout << "vacio\n";
}

template <class T>
void CMaxHeap3<T>::push(int x)
{
    heap.push_back(x);
    heapifyup(heap.size() - 1);
}

template <class T>
void CMaxHeap3<T>::pop()
{
    if (heap.empty()) return;
    heap[0] = heap.back();
    heap.pop_back();
    heapfyDown(0);
}

template<class T>
void CMaxHeap3<T>::heapifyup(int i) {
    while (i > 0) {
        int padre = (i - 1) / 3;
        if (heap[i] > heap[padre]) {
            std::swap(heap[i], heap[padre]);
            i = padre;
        }
        else break;
    }
}

template<class T>
void CMaxHeap3<T>::heapfyDown(int i) {
    int n = heap.size();
    while (true) {
        int hijo1 = 3 * i + 1;
        int hijo2 = 3 * i + 2;
        int hijo3 = 3 * i + 3;
        int mayor = i;
        if (hijo1 < git pushn && heap[hijo1] > heap[mayor]) mayor = hijo1;
        if (hijo2 < n && heap[hijo2] > heap[mayor]) mayor = hijo2;
        if (hijo3 < n && heap[hijo3] > heap[mayor]) mayor = hijo3;

        if (mayor != i) {
            std::swap(heap[i], heap[mayor]);
            i = mayor;
        }
        else break;
    }
}

template <class T>
void CMaxHeap3<T>::print()
{
    for (auto i = heap.begin(); i != heap.end(); ++i)
        std::cout << *i << " ";
    std::cout << "\n";
}

int main()
{
    CMaxHeap3<int> h;
    std::vector<int> vpush = { 30,35,40,50,43,36,60,51,70,90,66,77,23,21,49,88,73 };
    for (auto i : vpush)
        h.push(i);
    h.print();
    for (int j = 0; j < 5; j++)
        h.pop();
    h.print();
}