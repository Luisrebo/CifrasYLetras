#ifndef TRIE_SOLVER_H_
#define TRIE_SOLVER_H_

#include "Trie.h"
#include "TrieQuery.h"
#include "FuncionesComunes.h"

class TrieSolver {
public:
	static  SolucionLetras solve(Trie::Link raiz, const std::string& letras);

private:
	static int MappingCharToPosition(char c);

	 static void explorarSolucion(Trie::Link nodo, TrieQuery& problema, SolucionLetras& solParcial);
	
};
#endif
