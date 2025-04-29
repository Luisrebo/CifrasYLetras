#include "HeuristicaPorPalabrasAlcanzables.h"


void HeuristicaPorPalabrasAlcanzables::prepararAtributosHeuristicaNodo(Trie::Link nodo) {

	prepararPalabrasAlcanzablesHeuristicaNodo(nodo);
}

//vamos recorriendo y expandiendo hacia arriba las plabras alcanzables
int HeuristicaPorPalabrasAlcanzables::prepararPalabrasAlcanzablesHeuristicaNodo(Trie::Link nodo) {

	if (!nodo)
		return 0;

	int sumaPalabrasAlcanzables = nodo->terminal? 1:0;
	for (auto hijo : nodo->hijos)
		if (hijo)
			sumaPalabrasAlcanzables += prepararPalabrasAlcanzablesHeuristicaNodo(hijo);

	nodo->numPalabrasAlcanzables = sumaPalabrasAlcanzables;
	return sumaPalabrasAlcanzables;
}
//ordenamos los hijos que vamos a recorrer buscando soluciones por su comparador de heuristica
void HeuristicaPorPalabrasAlcanzables::ordenaVectorHijosSegunHeuristica(Trie::Link nodo) const {

	if (!nodo)
		return;

	//los ordenamos en funcion al coparador que termina la heuristica
	std::sort(nodo->listaHijosOrdenadosPorHeuristica.begin(), nodo->listaHijosOrdenadosPorHeuristica.end(), getComparator());

	for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		if (hijo)
			ordenaVectorHijosSegunHeuristica(hijo);
}

