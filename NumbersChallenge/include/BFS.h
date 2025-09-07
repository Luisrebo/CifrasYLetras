#ifndef BFS_H
#define BFS_H

#include "commonFunctions.h"
#include "Search.h"
#include <queue>

class BFS :public Search {

public:

	BFS(const num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

	//void mostrarBFS();

	SearchResult busqueda(StatsSingleCaseCifras& stats) override;

private:

	/*ATB*/
	//int numObjetivo; ya lo tiene por search
	std::queue<Problema> cola; //cola de vertices / nodos del bfs, no exploramos el nivel i + 1 hasta tener explorado completamente el nivel i
	//num_t  mejorSolucion;//mejor solucion encontrada

	/*FUN*/
	void resolver();

};

#endif // BFS_H