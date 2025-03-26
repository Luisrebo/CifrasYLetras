#ifndef Search_H
#define Search_H

#include "FuncionesComunes.h"
#include <vector>
using namespace std;

struct Problema {
	Problema(num_t solParcial,
		std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso,
		std::array<char, CIFRAS_INICIALES> operacionesEnOrden,
		const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

	num_t _solParcial;
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	std::array<num_t, CIFRAS_INICIALES> _numCandidatos;
};
struct Solucion {
	Solucion(Problema problema, num_t solMejor, num_t nivel);
	Problema _problema;
	num_t _solMejor;
	num_t _nivel; //para reconstruir la solucion
};

//Para rastrear la solucion en dfs y devolver una estructura con operaciones y cifras
struct rastreoSolucion {
	rastreoSolucion(num_t operacionesEnOrdenSize, num_t ordenDeUsoSize);
	std::vector<char> _operacionesEnOrden;
	std::vector<num_t> _ordenDeUso;
	num_t indiceComienzoOperacionesEnOrden;
	num_t indiceComienzoOrdenDeUso;
};

class Search {

public:
	Search(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& nCandidatos);

	virtual ~Search();//(~ destructor) virtual es para herencia 

	//metodos a sobre escribir en bfs y dfs
	virtual SearchResult busqueda() = 0;//indicamos que son metodos que se deben a los hijos (pure virtual)


protected:
	// Atributos para dfs y bfs
	num_t numObjetivo;
	std::array<num_t, CIFRAS_INICIALES> numerosCandidatos;

	const array<Operacion, 4> OPERACIONES = {
   Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
   {'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
   {'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
   {'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a >= b && a % b == 0; } }
	};
};




#endif 
