#include "TrieSolver.h"

using namespace std;
SolucionLetras TrieSolver::solve(Trie::Link raiz, const string& letras, PromedioGlobal&promedioGlobalLetras) {

	TrieQuery problema(letras);
	SolucionLetras solParcial;
	StatsSingleCaseLetras statsSingleCase;

	explorarSolucion(raiz, problema, solParcial, statsSingleCase);

	//añadimos al promedio global las stats de la resolucion de este caso
	promedioGlobalLetras.updateStatsToGlobal(statsSingleCase.statsComunes);

	problema.imprimirSolucion();

	return problema.mejorSolucion;
}

void TrieSolver::explorarSolucion(Trie::Link nodo, TrieQuery &problema, SolucionLetras &solParcial, StatsSingleCaseLetras &statsSingleCase) {

	if (problema.mejorSolucion.longitud == NUMERO_MAXIMO_LETRAS_EN_PALABRA)
		return;

	//estamos visitando un nuevo nodo
	statsSingleCase.statsComunes.numeroDeNodosVisitados += 1;

	//recorremos los hijos dando prioridad segurn la heuristica por la cual los ordené
	for (auto nodoHijo : nodo->listaHijosOrdenadosPorHeuristica) {

		if (nodoHijo != nullptr) {
			if (problema.mejorSolucion.longitud == NUMERO_MAXIMO_LETRAS_EN_PALABRA)
				return;

			//si en el input tenemos la letra que representa el nodo y no la hemos agotado en nieveles superiores 
			//tamb hacemos una poda en la que si la longitud de la solucion parcial que llevamos + la longitud maxima alcanzable para la palabra mas larga que podriamos
			//llegar a obtener es aun asi peor que la mejor sol que ya tenemos saltamos este nodo y exploramos el siguiente
			if (problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] > 0 && problema.mejorSolucion.longitud < (nodoHijo->nivel + nodoHijo->altura)) {

				//marcadores
				solParcial.palabraSolucion[nodo->nivel] = nodoHijo->elem;
				solParcial.longitud = nodo->nivel + 1;
				problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] -= 1;

				//si tenemos una solucion mas larga y es una palabra de nuestro vocabulario
				if (nodoHijo->nivel > problema.mejorSolucion.longitud && nodoHijo->terminal) {
					problema.mejorSolucion = solParcial;

					//contemplamos en las stats la altura de la nueva solucion y que acabamos de realizar una actualizacion de ella 
					statsSingleCase.statsComunes.nivelSolucion = problema.mejorSolucion.longitud;
					statsSingleCase.statsComunes.numeroDeVecesActualizaSolucion += 1;
					

					if (problema.mejorSolucion.longitud == NUMERO_MAXIMO_LETRAS_EN_PALABRA)
						return;
				}

				explorarSolucion(nodoHijo, problema, solParcial, statsSingleCase);
				solParcial.longitud = nodo->nivel;
				problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] += 1;

			}
		}
	}
	//antes de retornar actualizamos ya que este nodo esta completamente explorado al estarlo todos sus hijos
	statsSingleCase.statsComunes.numeroDeNodosCompletamenteExplorados += 1;
	return;
}

//si indexamos los vértices por la letra que ocupan en el vector de letras disponibles podemos saber si una letra esta disponible 
//para usarla mirando directamente la posicion que ocupa a traves de su ascci sabiendo que los ascii de las letras son consecutivas
//al igual que sus posiciones empezando en [0]='a'
int TrieSolver::MappingCharToPosition(char c) {
	return c - 'a';
}