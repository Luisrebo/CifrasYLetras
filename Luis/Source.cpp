#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>
#include <queue>
#include <algorithm>
#include <string>
#include <cctype>
#include <functional>  // lo necesito para std::function

#include "FuncionesComunes.h"
#include "BFS.h"
#include "DFS.h"
#include "Factory.h"

/*Segunda parte letras*/
#include "Trie.h"

using namespace std;



void resuelveCaso(string mode) {

	//numero al que queremos llegar o aproximarnos lo maximo posible
	int numObjetivo;

	//cifras con las que hemos de operar
	vector<int> numerosCandidatos(CIFRAS_INICIALES);

	cin >> numObjetivo;

	for (int i = 0; i < CIFRAS_INICIALES; ++i)
		cin >> numerosCandidatos[i];

	auto search = Factory::create(mode, numObjetivo, numerosCandidatos);

	search->busqueda();

}
/*Segunda Parte Letras*/
void resuelveCasoLetras() {
	int numPalabras;
	cin >> numPalabras;

	Trie arbolPrefijos;
	string palabra;//IMPORTANTE HACER IGNORE CASE AQUI O EN LA CLASE TRIE?
	for (int i = 0; i < numPalabras; ++i) {//podriamos ignorar las palabras de mas de 10 letras 
		cin >> palabra;
		arbolPrefijos.insert(palabra);
	}

	//caso para resolver;
	
	string letrasCasoi;
	int numProblemas;
	cin >> numProblemas;

	for (int i = 0; i < numProblemas; ++i) {
		cin >> letrasCasoi;

		arbolPrefijos.solve(letrasCasoi);
	}
		

}



//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main(int argc, char* argv[]) {
	// ajustes para que cin extraiga directamente de un fichero

	// Verificp si se paso un archivo como argumento
	if (argc < 2) {
		cerr << "Uso: " << argv[0] << " <archivo_de_entrada.txt>" << endl;
		return 1;
	}

	string nombreArchivo = argv[1];

	// Abrimos el archivo
	ifstream archivoEntrada(nombreArchivo);
	if (!archivoEntrada) {
		cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
		return 1;
	}

	// Redirigir la entrada estándar desde el archivo
	auto cinbuf = cin.rdbuf(archivoEntrada.rdbuf());
	/*
#ifndef DOMJUDGE
	std::ifstream in("Texto.txt");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif*/

	int numCasos;
	string mode;

	std::cin >> numCasos;
	cin >> mode;

	//tiempo antes de ejecutar el algoritmo
	auto start = std::chrono::high_resolution_clock::now();

	cout << "Solucion para el archivo: " << nombreArchivo << " en modo " << mode << '\n';

	for (int i = 0; i < numCasos; ++i)
		resuelveCaso(mode);

	/*Segunda parte letras*/
	resuelveCasoLetras();

	//guardamos el tiempo despues de ejecutar el algoritmo
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "Tiempo en la resolucion de los " << numCasos << " casos de prueba  " << duration.count() << " milisegundos.\n";

	// para dejar todo como estaba al principio
	 /*
#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	system("PAUSE");
#endif*/
	return 0;
}
