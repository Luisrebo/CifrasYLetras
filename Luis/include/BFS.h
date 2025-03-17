#ifndef BFS_H
#define BFS_H

#include "FuncionesComunes.h"

#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <queue>
#include <functional> 
#include <vector>
#include "Search.h"
using namespace std;

class BFS :public Search {

public:

	BFS(const num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

	void mostrarBFS();

	SearchResult busqueda() override;

private:

	/*ATB*/
	//int numObjetivo; ya lo tiene por search
	queue<Problema> cola; //cola de vertices / nodos del bfs, no exploramos el nivel i + 1 hasta tener explorado completamente el nivel i
	num_t  mejorSolucion;//mejor solucion encontrada
	Problema verticeOrigen;
	Solucion sol;

	/*FUN*/
	void resolver();

};

#endif // BFS_H