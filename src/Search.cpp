#include "Search.h"


Problema::Problema(num_t solParcial,
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso,
	std::array<char, CIFRAS_INICIALES> operacionesEnOrden,
	const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos)
	: _solParcial(solParcial),
	_ordenDeUso(ordenDeUso),
	_operacionesEnOrden(operacionesEnOrden),
	_numCandidatos(numerosCandidatos) {
}

Solucion::Solucion(Problema problema,
	num_t solMejor, num_t nivel) :_problema(problema), _solMejor(solMejor), _nivel(nivel) {}

rastreoSolucion::rastreoSolucion(num_t operacionesEnOrdenSize, num_t ordenDeUsoSize)
	: _operacionesEnOrden(operacionesEnOrdenSize), _ordenDeUso(ordenDeUsoSize),
	indiceComienzoOperacionesEnOrden(operacionesEnOrdenSize - 1), indiceComienzoOrdenDeUso(ordenDeUsoSize - 1) {}

Search::Search(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& nCandidatos)
	: numObjetivo(nObjetivo), numerosCandidatos(nCandidatos) {}

Search::~Search() {}