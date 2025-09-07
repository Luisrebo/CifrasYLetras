#include "HeightHeuristic.h"
#include <algorithm>

//aqui no es necesario porque ya esta la altura actualizada se hace en IHeuristica preparar para todos
void HeightHeuristic::prepararAtributosHeuristicaNodo(Trie::Link) {}



void HeightHeuristic::ordenaVectorHijosSegunHeuristica(Trie::Link nodo) const {

	if (!nodo)
		return;

	//los ordenamos en funcion al coparador que termina la heuristica
	std::sort(nodo->listaHijosOrdenadosPorHeuristica.begin(), nodo->listaHijosOrdenadosPorHeuristica.end(), getComparator());

	for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		if (hijo)
			ordenaVectorHijosSegunHeuristica(hijo);
}