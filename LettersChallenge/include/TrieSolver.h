#ifndef TRIE_SOLVER_H
#define TRIE_SOLVER_H

#include "Trie.h"
#include "TrieQuery.h"
#include "commonFunctions.h"
#include "GlobalAverage.h"

class TrieSolver {

public:
	static  SolucionLetras solve(Trie::Link raiz, const std::string& letras, GlobalAverage&promedioGlobalLetras);

private:
	static int MappingCharToPosition(char c);

	 static void explorarSolucion(Trie::Link nodo, TrieQuery& problema, SolucionLetras& solParcial, StatsSingleCaseLetras &statsSingleCase);
	
};
#endif
