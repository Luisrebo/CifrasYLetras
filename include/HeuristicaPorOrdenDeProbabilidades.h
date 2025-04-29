#ifndef HEURISTICA_POR_ORDEN_DE_PROBABILIDADES_H
#define HEURISTICA_POR_ORDEN_DE_PROBABILIDADES_H


#include "IHeuristica.h"



class HeuristicaPorOrdenDeProbabilidades : public IHeuristica {

public:

	HeuristicaPorOrdenDeProbabilidades(MatricesDeProbabilidades const& matrizProbs) : matrizProbabilidades(matrizProbs) {}
	//preparo los atrb de los nodos extendiendolos hacia sus padres, para la ordenacion derivada de la heuristica por eejemplo palabras alcanzables...
	void prepararAtributosHeuristicaNodo(Trie::Link nodo) override;

	void ordenaVectorHijosSegunHeuristica(Trie::Link nodo)const override;


	std::string nombre() const override { return "Heuristica Por Probabilidades "; }

	virtual ~HeuristicaPorOrdenDeProbabilidades() = default;

protected:

	MatricesDeProbabilidades matrizProbabilidades;

	// Este es el paso que faltaba: sobreescribir getComparator
	function<bool(Trie::Link, Trie::Link)> getComparator() const override {
		return [](Trie::Link, Trie::Link) {
			return false;
			};
	}
	
	//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas numero de palabras
	struct HeuristicaComparador {

		MatricesDeProbabilidades _matriz;
		int _nivel; //nivel del arbol
		int _IndiceSucesora; //indice de la letra sucesora
		HeuristicaComparador(MatricesDeProbabilidades const& matriz, int nivel, int sucesora) :_matriz(matriz), _nivel(nivel), _IndiceSucesora(sucesora) {}

		bool operator()(Trie::Link const l1, Trie::Link const l2) const {


			double pesoLetraAntecesora = 0;
			double pesoLetraPorNivel = 1;
			double puntuacionLetraAntecesora1 = 0, puntuacionLetraAntecesora2 = 0;
			double puntuacionLetraNivel1 = 0, puntuacionLetraNivel2 = 0;

			if (_nivel >= 1 && _nivel <= 9) {
				// niveles 1 al 9: 
				pesoLetraAntecesora = 0.75;
				pesoLetraPorNivel = 0.25;
				puntuacionLetraAntecesora1 = pesoLetraAntecesora
					* _matriz.probabilidadLetraPosterior[_IndiceSucesora][l1->elem - 'a'];
				puntuacionLetraAntecesora2 = pesoLetraAntecesora
					* _matriz.probabilidadLetraPosterior[_IndiceSucesora][l2->elem - 'a'];
				puntuacionLetraNivel1 = pesoLetraPorNivel
					* _matriz.probabilidadLetraPorNivel[_nivel][l1->elem - 'a'];
				puntuacionLetraNivel2 = pesoLetraPorNivel
					* _matriz.probabilidadLetraPorNivel[_nivel][l2->elem - 'a'];
			}
			else {
				// nivel 0
				// pesoLetraAntecesora = 0; pesoLetraPorNivel = 1; ya inicializado así
				puntuacionLetraNivel1 = pesoLetraPorNivel
					* _matriz.probabilidadLetraPorNivel[_nivel][l1->elem - 'a'];
				puntuacionLetraNivel2 = pesoLetraPorNivel
					* _matriz.probabilidadLetraPorNivel[_nivel][l2->elem - 'a'];
			}

			double puntuacionHeuristicaHio1 = puntuacionLetraAntecesora1 + puntuacionLetraNivel1;
			double puntuacionHeuristicaHio2 = puntuacionLetraAntecesora2 + puntuacionLetraNivel2;

			return puntuacionHeuristicaHio1 > puntuacionHeuristicaHio2;
		}
	};


};



#endif
