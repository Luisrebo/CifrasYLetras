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
