#include "AlphabeticalOrderHeuristic.h"
#include <algorithm>
//nada que preparar los hijos se insertan en orden al estar indexados por su char desde [0]=a
void AlphabeticalOrderHeuristic::prepararAtributosHeuristicaNodo(Trie::Link nodo) {}


//ordenamos los hijos que vamos a recorrer buscando soluciones por su comparador de heuristica
void AlphabeticalOrderHeuristic::ordenaVectorHijosSegunHeuristica(Trie::Link nodo) const {

	if (!nodo)
		return;

	//los ordenamos en funcion al coparador que termina la heuristica
	std::sort(nodo->listaHijosOrdenadosPorHeuristica.begin(), nodo->listaHijosOrdenadosPorHeuristica.end(), getComparator());

	for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		if (hijo)
			ordenaVectorHijosSegunHeuristica(hijo);
}