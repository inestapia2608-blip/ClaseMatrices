
#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <string>

class Matriz {
private:
    int numFilas;
    int numColumnas;

    std::vector<int>* indices;
    std::vector<double>* valores;

public:
    Matriz(const std::string &ruta);     // leer desde archivo
    Matriz(int filas, int columnas);     // matriz vacía
    Matriz(const Matriz &otra);
    Matriz& operator=(const Matriz &otra);
	~Matriz();

    double get(int i, int j) const;
    void set(int i, int j, double valor);
    void swapFilas(int f1, int f2);
    int buscarPivote(const Matriz &A, int col);
    

    int filas() const { return numFilas; }
    int columnas() const { return numColumnas; }

    Matriz resolverSistemaLU(const std::string &rutaVector);
};

#endif
