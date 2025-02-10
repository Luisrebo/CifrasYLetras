#ifndef Search_H
#define Search_H

#include "FuncionesComunes.h"
#include <vector>
using namespace std;

class Search {

public:
	Search(int nObjetivo, vector<int>& nCandidatos);

	virtual ~Search();//(~ destructor) virtual es para herencia 

	//metodos a sobre escribir en bfs y dfs
	virtual void busqueda() = 0;//indicamos que son metodos que se deben a los hijos (pure virtual)


protected:
	// Atributos para dfs y bfs
	int numObjetivo;
	std::vector<int> numerosCandidatos;
	const array<Operacion, 4> OPERACIONES = {
   Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
   {'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
   {'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
   {'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; } }
	};
};




#endif 
