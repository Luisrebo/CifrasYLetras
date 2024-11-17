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
using namespace std;

struct SolucionBFS {
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	num_t solMejor;
	num_t _nivel; //para reconstruir la solucion
};


struct Problema {
	Problema(num_t solParcial,
		std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso,
		std::array<char, CIFRAS_INICIALES> operacionesEnOrden,
		std::array<num_t, CIFRAS_INICIALES> numCandidatos,
		num_t nivel);

	num_t _nivel;
	num_t _solParcial;
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	std::array<num_t, CIFRAS_INICIALES> _numCandidatos;
};

class BFS {
public:

	BFS(const int& nObjetivo, vector<int> numerosCandidatos);

	void mostrarBFS();

private:

	/*ATB*/
	SolucionBFS sol;
	int numObjetivo;
	queue<Problema> cola; //cola de vertices / nodos del bfs, no exploramos el nivel i + 1 hasta tener explorado completamente el nivel i
	num_t  mejorSolucion;//mejor solucion encontrada
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso;//orden de uso de las cifras el conseguir una solucion(para la reconstruccion de la solucion)/
	std::array<char, CIFRAS_INICIALES>operacionesEnOrden;//lista de la operaciones en orden de uso (para reconstruir la solucion)/
	std::array<num_t, CIFRAS_INICIALES> numCandidatos; /*Numeros candidatos pero en array estatico*/

	const array<Operacion, 4> OPERACIONES = {
	   Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
	   {'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
	   {'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
	   {'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; } }
	};

	/*FUN*/
	void resolver();



};

#endif // BFS_H