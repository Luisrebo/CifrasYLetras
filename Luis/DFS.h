#ifndef DFS_H
#define DFS_H

#include "FuncionesComunes.h"
#include <vector>
#include <array>
#include "Search.h"
using namespace std;


class DFS :public Search {
public:

	DFS(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

	void mostrarDFS();

	void busqueda() override;

private:

	/*ATR*/
	
	Problema problemaBase;
	Solucion sol;
	

	//void resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol);
	//void resolver(int k, int nivel, int solParcial /* ,int& mejorSol*/ /*, vector<bool>& marcador*/ /*, const int numObjetivo*/, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden /*, SolucionDFS& sol*/);
	
	void resolvere(int nivel, array<num_t, CIFRAS_INICIALES>& numerosCandidatos, array <num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordenDeUso, array <char, CIFRAS_INICIALES>& operacionesEnOrden);
	
	bool contained(vector<int> ordenDeUso, int cifra, int solMejor);


};

#endif// DFS_H