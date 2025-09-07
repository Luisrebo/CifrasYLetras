#ifndef HEIGHT_HEURISTIC_H
#define HEIGHT_HEURISTIC_H


#include "IHeuristic.h"
#include <functional>

//En esta Heuristica recorremos primero los nodos con mayor altura
class HeightHeuristic : public IHeuristic {
public:
	
	void prepararAtributosHeuristicaNodo(Trie::Link nodo) override;
	
	std::string nombre() const override { return "Heuristica Por Altura"; }

	void ordenaVectorHijosSegunHeuristica(Trie::Link nodo)const override;

	virtual ~HeightHeuristic() = default;

protected:

	// Este es el paso que faltaba: sobreescribir getComparator
	std::function<bool(Trie::Link, Trie::Link)> getComparator() const override {
		return highComparator();
	}

	//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas altura , mas profundos
	struct highComparator {
		bool operator()(Trie::Link const l1, Trie::Link const l2) const {

			// Ambos nulos se mantienen en posicion
			if (l1 == nullptr && l2 == nullptr) return false;
			//Si solo uno es no nulo va delante:
			if (l1 == nullptr) return false;  // l1 nulo va despues
			if (l2 == nullptr) return true;   // l2 nulo va despues

			if (l1->altura == l2->altura)
				return l1->elem < l2->elem; // desempatedeterminista?
			return l1->altura > l2->altura;
		}
	};
};
#endif