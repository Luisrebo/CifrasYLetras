#ifndef DFS_H
#define DFS_H

#include "commonFunctions.h"
#include "Search.h"


class DFS :public Search {
public:

	DFS(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

	SearchResult busqueda(StatsSingleCaseCifras &stats) override;

private:

	/*ATR*/
	//void resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol);
	//void resolver(int k, int nivel, int solParcial /* ,int& mejorSol*/ /*, vector<bool>& marcador*/ /*, const int numObjetivo*/, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden /*, SolucionDFS& sol*/);

	void resolvere(int nivel, std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos, std::array <num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordenDeUso, std::array <char, CIFRAS_INICIALES>& operacionesEnOrden);

	bool contained(std::array<int, CIFRAS_INICIALES>& ordenDeUso, int cifra, int solMejor);

};

#endif// DFS_H