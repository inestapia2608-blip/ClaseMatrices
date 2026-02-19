#include <Matriz.h>
using namespace std;

Matriz::Matriz(const string &ruta){
	ifstream archivo(ruta);
	
	if(!archivo.is_open()){
		throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
	}
	
	archivo>>numFilas>>numColumnas;
	
	indices= new vector<int> [numFilas];
	valores= new vector<double> [numFilas];
	
	for (int i=0; i<numFilas; i++){
		int k;
		archivo>>k;
		
		//alternativa con indices[i].push_back(valor leido) dentro de los for/ ligeramente más lento
		//da a los vectores en los arreglos de indices el tamaño que necesitaran para poder acceder con [i][j]
		indices[i].resize(k);
		valores[i].resize(k);
		
		for (int j=0; j<k; j++){
			archivo>>indices[j];
			indices[j]--;
			
		}
		
		for (int j=0; j<k; j++){
			archivo>>valores[j];
		}
	}
	
	archivo.close();
	
}
