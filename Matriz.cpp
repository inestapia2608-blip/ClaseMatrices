#include "Matriz.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>   // necesario para std::swap

using namespace std;

//// CONSTRUCTOR DESDE ARCHIVO
Matriz::Matriz(const string &ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open())
        throw runtime_error("No se pudo abrir el archivo");

    archivo >> numFilas >> numColumnas;

    indices = new vector<int>[numFilas];
    valores = new vector<double>[numFilas];

    for (int i = 0; i < numFilas; i++) {
        int k;
        archivo >> k;

        vector<int> cols(k);
        vector<double> vals(k);

        // leer columnas
        for (int j = 0; j < k; j++)
            archivo >> cols[j];

        // leer valores
        for (int j = 0; j < k; j++)
            archivo >> vals[j];

        indices[i] = cols;
        valores[i] = vals;
    }

    archivo.close();
}

//// CONSTRUCTOR VACÍO
Matriz::Matriz(int filas, int columnas) {
    numFilas = filas;
    numColumnas = columnas;
    indices = new vector<int>[filas];
    valores = new vector<double>[filas];
}

Matriz::Matriz(const Matriz &otra) {
    numFilas = otra.numFilas;
    numColumnas = otra.numColumnas;

    indices = new vector<int>[numFilas];
    valores = new vector<double>[numFilas];

    for (int i = 0; i < numFilas; i++) {
        indices[i] = otra.indices[i];
        valores[i] = otra.valores[i];
    }
}

Matriz& Matriz::operator=(const Matriz &otra) {

    if (this == &otra)
        return *this;

    delete[] indices;
    delete[] valores;

    numFilas = otra.numFilas;
    numColumnas = otra.numColumnas;

    indices = new vector<int>[numFilas];
    valores = new vector<double>[numFilas];

    for (int i = 0; i < numFilas; i++) {
        indices[i] = otra.indices[i];
        valores[i] = otra.valores[i];
    }

    return *this;
}

//// DESTRUCTOR
Matriz::~Matriz() {
    delete[] indices;
    delete[] valores;
}

//// OBTENER VALOR
double Matriz::get(int i, int j) const {
    for (size_t k = 0; k < indices[i].size(); k++)
        if (indices[i][k] == j)
            return valores[i][k];
    return 0.0;
}

//// INSERTAR VALOR (si ?0)
void Matriz::set(int i, int j, double valor) {
    if (valor == 0) return;
    indices[i].push_back(j);
    valores[i].push_back(valor);
}

//// LEER VECTOR b
vector<double> leerVector(const string &ruta) {
    ifstream archivo(ruta);

    if (!archivo.is_open())
        throw runtime_error("No se pudo abrir el vector");

    int n;
    archivo >> n;

    vector<double> b(n);
    for (int i = 0; i < n; i++)
        archivo >> b[i];

    return b;
}

//// SWAP FILAS
void Matriz::swapFilas(int f1, int f2) {
    std::swap(indices[f1], indices[f2]);
    std::swap(valores[f1], valores[f2]);
}

//// BUSCAR PIVOTE
int buscarPivote(const Matriz &A, int col) {
    int n = A.filas();

    for (int i = col; i < n; i++) {
        if (A.get(i,col) != 0)
            return i;
    }
    return -1;
}
//// FACTORIZACIÓN LU (Doolittle)
void factorizarLU(Matriz &A, Matriz &L, Matriz &U) {

    int n = A.filas();

    for (int i = 0; i < n; i++) {

        // ?? PIVOTEO PARCIAL
        if (A.get(i,i) == 0) {

            int filaPivote = buscarPivote(A, i);

            if (filaPivote == -1)
                throw runtime_error("Matriz singular: no hay pivote");

            A.swapFilas(i, filaPivote);
        }

        // U
        for (int k = i; k < n; k++) {
            double suma = 0;
            for (int j = 0; j < i; j++)
                suma += L.get(i,j) * U.get(j,k);

            double val = A.get(i,k) - suma;
            if (val != 0) U.set(i,k,val);
        }

        // L
        for (int k = i; k < n; k++) {
            if (i == k)
                L.set(i,i,1);
            else {
                double suma = 0;
                for (int j = 0; j < i; j++)
                    suma += L.get(k,j) * U.get(j,i);

                double pivote = U.get(i,i);

                if (pivote == 0)
                    throw runtime_error("Pivote cero durante LU");

                double val = (A.get(k,i) - suma) / pivote;
                if (val != 0) L.set(k,i,val);
            }
        }
    }
}

//// SUSTITUCIÓN ADELANTE (Ly=b)
vector<double> forwardSub(const Matriz &L, const vector<double> &b) {
    int n = b.size();
    vector<double> y(n);

    for (int i = 0; i < n; i++) {
        double suma = 0;
        for (int j = 0; j < i; j++)
            suma += L.get(i,j) * y[j];

        y[i] = b[i] - suma;
    }
    return y;
}

//// SUSTITUCIÓN ATRÁS (Ux=y)
vector<double> backwardSub(const Matriz &U, const vector<double> &y) {
    int n = y.size();
    vector<double> x(n);

    for (int i = n-1; i >= 0; i--) {
        double suma = 0;
        for (int j = i+1; j < n; j++)
            suma += U.get(i,j) * x[j];

        x[i] = (y[i] - suma) / U.get(i,i);
    }
    return x;
}

//// RESOLVER Ax=b
Matriz Matriz::resolverSistemaLU(const string &rutaVector) {

    vector<double> b = leerVector(rutaVector);

    if (b.size() != (size_t)numFilas)
        throw runtime_error("Dimensiones incompatibles");

    Matriz L(numFilas, numColumnas);
    Matriz U(numFilas, numColumnas);

    Matriz copia = *this;   // no modificar original
	factorizarLU(copia, L, U);

    vector<double> y = forwardSub(L,b);
    vector<double> x = backwardSub(U,y);

    // devolver como matriz columna
    Matriz solucion(numFilas, 1);
    for (int i = 0; i < numFilas; i++)
        solucion.set(i,0,x[i]);

    return solucion;
}
