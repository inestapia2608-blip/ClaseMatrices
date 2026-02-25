#include <iostream>
#include "Matriz.h"

using namespace std;

int main() {
    try {
        Matriz A("X.txt");

        Matriz x = A.resolverSistemaLU("r.txt");

        cout << "Solución:\n";
        for (int i = 0; i < x.filas(); i++)
            cout << x.get(i,0) << endl;

    } catch (exception &e) {
        cout << e.what() << endl;
    }

    return 0;
}
