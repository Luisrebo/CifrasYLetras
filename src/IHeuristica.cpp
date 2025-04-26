#include "IHeuristica.h"
#include <algorithm>

//preparamos para recorrer cada nodo del arbol, tanto los atributos como las alturas de sus nodos y su vector de heuristica
void IHeuristica::preparar(Trie::Link nodo) {

	prepararAlturasNodo(nodo);//actualiza todas las alturas de todos los nodos del trie 

	//override en cada heuristica
	prepararAtributosHeuristicaNodo(nodo);//actualiza los atributos de los nosodos del trie que vayan a servir para la heuristica concret (palabras alcanzables)

	prepareTrieParaExplorarPorHeuristica(nodo);

}

int IHeuristica::prepararAlturasNodo(Trie::Link nodo) {

	if (!nodo)
		return 0;

	int maxAlt = 1;
	for (auto hijo : nodo->hijos)
		if (hijo)
			maxAlt = max(maxAlt, prepararAlturasNodo(hijo) + 1);

	nodo->altura = maxAlt;
	return nodo->altura;
}


void IHeuristica::prepareTrieParaExplorarPorHeuristica(Trie::Link nodo) {

	fillVectoresDeNodosSegunHeuristica(nodo);//recorre todos los nodos del arbol rellenando los vectores de cada nodo que van a tener sus hijos ordenados por el comparador de la heuristica

	ordenaVectorHijosSegunHeuristica(nodo);//ordena el vector de hijos segun heuristica que va a tener  

	
}

//rellenamos`para cada nodo su vector de hijos sobre el cual exploraremos el arbol atendiendo a la heuristica
void IHeuristica::fillVectoresDeNodosSegunHeuristica(Trie::Link nodo){
	if (!nodo)
		return;

	for (auto hijo : nodo->hijos)
		if (hijo)
			nodo->listaHijosOrdenadosPorHeuristica.push_back(hijo);

	for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		fillVectoresDeNodosSegunHeuristica(hijo);
}
 //ordenamos los hijos que vamos a recorrer buscando soluciones por su comparador de heuristica
void IHeuristica::ordenaVectorHijosSegunHeuristica(Trie::Link nodo) {

	 if (!nodo)
		 return;

	 //los ordenamos en funcion al coparador que termina la heuristica
	 std::sort(nodo->listaHijosOrdenadosPorHeuristica.begin(), nodo->listaHijosOrdenadosPorHeuristica.end(), getComparator());

	 for (auto hijo : nodo->listaHijosOrdenadosPorHeuristica)
		 if (hijo)
			 ordenaVectorHijosSegunHeuristica(hijo);
 }

