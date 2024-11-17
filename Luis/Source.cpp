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
using namespace std;

num_t calcular(char operacion, num_t solucionParcial, num_t operando) {
	switch (operacion) {
	case '+':
		return solucionParcial + operando;
	case '-':
		return solucionParcial - operando;
	case '*':
		return solucionParcial * operando;
	case '/':
		if (operando != 0) {
			return solucionParcial / operando;
		}
		else {
			return 0; // Evitar división por cero
		}
	default:
		return solucionParcial;
	}
}
bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
	if (str1.length() != str2.length()) {
		return false;
	}

	return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
		return std::tolower(a) == std::tolower(b);
		});
}
void resuelveCaso(string mode) {
	int numObjetivo;


	vector<int> numerosCandidatos(6);

	cin >> numObjetivo;


	for (int i = 0; i < 6; i++)
		cin >> numerosCandidatos[i];

	if (equalsIgnoreCase(mode, DFS_MODE)) {
		DFS dfs(numObjetivo, numerosCandidatos);
		dfs.mostrarDFS();

	}

	if (equalsIgnoreCase(mode, BFS_MODE)) {
		BFS bfs(numObjetivo, numerosCandidatos);
		bfs.mostrarBFS();

	}



}



//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main() {
	// ajustes para que cin extraiga directamente de un fichero


#ifndef DOMJUDGE
	std::ifstream in("Texto.txt");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

	int numCasos;
	string mode;

	std::cin >> numCasos;
	cin >> mode;

	//tiempo antes de ejecutar el algoritmo
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numCasos; ++i)
		resuelveCaso(mode);

	//guardamos el tiempo despues de ejecutar el algoritmo
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "Tiempo en la resolucion de los " << numCasos << " casos de prueba  " << duration.count() << " milisegundos.\n";

	// para dejar todo como estaba al principio
#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	system("PAUSE");
#endif
	return 0;
}
