#ifndef DFS_H
#define DFS_H

#include "FuncionesComunes.h"
#include <vector>
#include "Search.h"
using namespace std;

struct SolucionDFS {//mejor inicializar en el constructor?
	vector<int> ordenDeUso;
	vector<char> operacionesEnOrden;
	int solMejor=0;
};

class DFS :public Search {
public:

	DFS(int nObjetivo, vector<int>  &nCandidatos);

	void mostrarDFS();

	void busqueda() override;

private:

	/*ATR*/
	SolucionDFS sol;
	int mejorSol = 0;
	vector<bool> marcador;
	vector<char> operacionesEnOrden;
	vector<int> ordenDeUso;
	vector<int> numerosCandidatos;
	int numObjetivo;

	//void resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol);
	void resolver(int k, int nivel, int solParcial /* ,int& mejorSol*/ /*, vector<bool>& marcador*/ /*, const int numObjetivo*/, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden /*, SolucionDFS& sol*/);

	bool contained(vector<int> ordenDeUso, int cifra, int solMejor);


};

#endif// DFS_H