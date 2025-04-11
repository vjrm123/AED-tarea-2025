#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

class V {
private:
    vector<int> d;
public:
    void push_back(int v) { d.push_back(v); }
    int size() const { return d.size(); }
    int& operator[](int i) { return d[i]; }
    const int& operator[](int i) const { return d[i]; }
};

void directo(V& v) {
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (v[j] > v[j + 1]) {
                int t = v[j];
                v[j] = v[j + 1];
                v[j + 1] = t;
            }
        }
    }
}

void funcion(V& v, bool (*cmp)(int, int)) {
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (cmp(v[j], v[j + 1])) {
                int t = v[j];
                v[j] = v[j + 1];
                v[j + 1] = t;
            }
        }
    }
}

bool c(int a, int b) { return (a % 10 + a) > (b % 10 + b); }
bool asc(int a, int b) { return a > b; }

class F {
private:
    bool (*cmp)(int, int);
public:
    F(bool (*f)(int, int)) : cmp(f) {}
    void operator()(V& v) {
        int n = v.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (cmp(v[j], v[j + 1])) {
                    int t = v[j];
                    v[j] = v[j + 1];
                    v[j + 1] = t;
                }
            }
        }
    }
};

class O {
public:
    virtual void ord(V& v) = 0;
    virtual ~O() {}
};

class Polimorfismo : public O {
public:
    void ord(V& v) override {
        int n = v.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - i - 1; ++j) {
                if (c(v[j], v[j + 1])) {
                    int t = v[j];
                    v[j] = v[j + 1];
                    v[j + 1] = t;
                }
            }
        }
    }
};

V crear(int n) {
    V v;
    random_device r;
    mt19937 g(r());
    uniform_int_distribution<> d(1, 1000);
    for (int i = 0; i < n; ++i) {
        v.push_back(d(g));
    }
    return v;
}

void test() {
    vector<int> tams = { 5000, 8000, 12000, 15000 };
    const int rep = 3;
    cout << "RESULTADOS\n";
    cout << "Tam\tDirecto\tFuncion\tFunctor\tPolimorf.\n";
    cout << "----------------------------------------------\n";
    for (int t : tams) {
        V ori = crear(t);
        F f(asc);
        Polimorfismo p;
        double td = 0, tf = 0, tfu = 0, tp = 0;
        for (int r = 0; r < rep; ++r) {
            V c;

            c = ori;
            auto ini = high_resolution_clock::now();
            directo(c);
            auto fin = high_resolution_clock::now();
            td += duration_cast<milliseconds>(fin - ini).count();

            c = ori;
            ini = high_resolution_clock::now();
            funcion(c, asc);
            fin = high_resolution_clock::now();
            tf += duration_cast<milliseconds>(fin - ini).count();

            c = ori;
            ini = high_resolution_clock::now();
            f(c);
            fin = high_resolution_clock::now();
            tfu += duration_cast<milliseconds>(fin - ini).count();

            c = ori;
            ini = high_resolution_clock::now();
            O* o = new Polimorfismo();
            o->ord(c);
            delete o;
            fin = high_resolution_clock::now();
            tp += duration_cast<milliseconds>(fin - ini).count();
        }

        cout << t << "\t" << td / rep << "\t" << tf / rep << "\t" << tfu / rep << "\t" << tp / rep << "\n";
    }
}

int main() {
    test();
    return 0;
}

