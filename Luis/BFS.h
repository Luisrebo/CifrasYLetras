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

/*struct SolucionBFS {
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	num_t solMejor;
	num_t _nivel; //para reconstruir la solucion
};*/

struct Problema {
	Problema(num_t solParcial,
		std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso,
		std::array<char, CIFRAS_INICIALES> operacionesEnOrden,
		vector<int> &numerosCandidatos);

	num_t _solParcial;
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	std::array<num_t, CIFRAS_INICIALES> _numCandidatos;
};
struct SolucionBFS {
	SolucionBFS(Problema problema, num_t solMejor, num_t nivel);
	Problema _problema;
	num_t _solMejor;
	num_t _nivel; //para reconstruir la solucion
};

class BFS :public Search {
	
public:

	BFS(const int nObjetivo, vector<int> &numerosCandidatos);

	void mostrarBFS();

	void busqueda() override;

private:

	/*ATB*/
	int numObjetivo;
	queue<Problema> cola; //cola de vertices / nodos del bfs, no exploramos el nivel i + 1 hasta tener explorado completamente el nivel i
	num_t  mejorSolucion;//mejor solucion encontrada

	Problema verticeOrigen;
	/*std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso;//orden de uso de las cifras el conseguir una solucion(para la reconstruccion de la solucion)/
	std::array<char, CIFRAS_INICIALES>operacionesEnOrden;//lista de la operaciones en orden de uso (para reconstruir la solucion)/
	std::array<num_t, CIFRAS_INICIALES> numCandidatos;*/ /*Numeros candidatos pero en array estatico*/

	SolucionBFS sol;


	/*FUN*/
	void resolver();



};

#endif // BFS_H