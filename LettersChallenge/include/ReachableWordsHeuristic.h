#ifndef REACHABLE_WORDS_HEURISTIC_H
#define REACHABLE_WORDS_HEURISTIC_H

#include <functional>
#include "IHeuristic.h"

class ReachableWordsHeuristic: public IHeuristic{

public:

	//preparo los atrb de los nodos extendiendolos hacia sus padres, para la ordenacion derivada de la heuristica por eejemplo palabras alcanzables...
	void prepararAtributosHeuristicaNodo(Trie::Link nodo) override;

	int prepararPalabrasAlcanzablesHeuristicaNodo(Trie::Link nodo) ;

	void ordenaVectorHijosSegunHeuristica(Trie::Link nodo)const override;

	std::string nombre() const override { return "Heuristica Por Palabras Alcanzables"; }

	virtual ~ReachableWordsHeuristic() = default;

protected:

	// Este es el paso que faltaba: sobreescribir getComparator
	std::function<bool(Trie::Link, Trie::Link)> getComparator() const override {
		return ReachableWordComparator();
	}

	//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas numero de palabras
	struct ReachableWordComparator {
		bool operator()(Trie::Link const l1, Trie::Link const l2) const {
			// Ambos nulos se mantienen en posicion
			if (l1 == nullptr && l2 == nullptr) return false;
			//Si solo uno es no nulo va delante:
			if (l1 == nullptr) return false;  // l1 nulo va despues
			if (l2 == nullptr) return true;   // l2 nulo va despues
			if (l1->numPalabrasAlcanzables == l2->numPalabrasAlcanzables)
				return l1->elem < l2->elem; // determinista
			return l1->numPalabrasAlcanzables > l2->numPalabrasAlcanzables;
		}
	};
};
#endif
