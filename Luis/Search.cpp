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

Search::Search(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& nCandidatos)
	: numObjetivo(nObjetivo), numerosCandidatos(nCandidatos) {}

Search::~Search() {}