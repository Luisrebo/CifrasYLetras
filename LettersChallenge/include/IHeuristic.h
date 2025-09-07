#ifndef IHEURISTIC_H
#define IHEURISTIC_H

#include <string>
#include <functional>
#include "Trie.h" 

class IHeuristic {

public:
	//preparamos el atrb propio de cada heuristica(Probabilidad, palabras alcanzables...) y la altura (necesaria en cualquier heuristica por las podas)
	virtual void preparar(Trie::Link nodo);

	virtual ~IHeuristic() = default;

protected:
	//preparo los atrb de los nodos extendiendolos hacia sus padres, para la ordenacion derivada de la heuristica por eejemplo palabras alcanzables...
	virtual void prepararAtributosHeuristicaNodo(Trie::Link) = 0;

	//actualizo/preparo  las alturas, es necesario siempre en cualquier heurística porque se usa para podar
	//Al hacerlo todas las heuristicas se define en cpp
	virtual int prepararAlturasNodo(Trie::Link);

	//Preparamos toda la heuristica
	 void prepareTrieParaExplorarPorHeuristica(Trie::Link);
	 void fillVectoresDeNodosSegunHeuristica(Trie::Link nodo);//recorre todos los nodos del arbol rellenando los vectores de cada nodo que van a tener sus hijos ordenados por el comparador de la heuristica
	 virtual void ordenaVectorHijosSegunHeuristica(Trie::Link nodo)const=0;//ordena el vector de hijos segun heuristica que va a tener  
	 
	//todas las heuristicas hacen lo mismo, primero rellenan el vector  de heuristica y luego lo ordenan por comparador
	virtual std::function<bool(Trie::Link, Trie::Link)> getComparator() const = 0;

	//nombre de la heuristica para imprimir con la solucion
	virtual std::string nombre() const = 0;

	
};



#endif
