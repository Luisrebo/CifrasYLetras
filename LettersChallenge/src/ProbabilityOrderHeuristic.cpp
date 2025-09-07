#include "ProbabilityOrderHeuristic.h"
#include <algorithm>

void ProbabilityOrderHeuristic::prepararAtributosHeuristicaNodo(Trie::Link nodo) {}

void ProbabilityOrderHeuristic::ordenaVectorHijosSegunHeuristica(Trie::Link nodo) const {

	if (!nodo)
		return;

	//los ordenamos en funcion al coparador que termina la heuristica
	std::sort(nodo->listaHijosOrdenadosPorHeuristica.begin(), nodo->listaHijosOrdenadosPorHeuristica.end(),
		ProbabilityOrderHeuristic::HeuristicaComparador { matrizProbabilidades,nodo->nivel, nodo->elem - 'a'});

	for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		if (hijo)
			ordenaVectorHijosSegunHeuristica(hijo);
}



