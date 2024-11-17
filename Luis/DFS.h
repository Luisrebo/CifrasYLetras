#ifndef DFS_H
#define DFS_H

#include "FuncionesComunes.h"
#include <vector>
using namespace std;

struct SolucionDFS {
	vector<int> ordenDeUso;
	vector<char> operacionesEnOrden;
	int solMejor;
};

class DFS {
public:

	DFS(int nObjetivo, const vector<int>  nCandidatos);

	void mostrarDFS();

private:

	/*ATR*/
	SolucionDFS sol;
	int mejorSol = 0;
	vector<bool> marcador;
	vector<char> operacionesEnOrden;
	vector<int> ordenDeUso;
	vector<int> numerosCandidatos;
	int numObjetivo;

	const array<Operacion, 4> OPERACIONES = {
	   Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
	   {'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
	   {'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
	   {'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; } }
	};

	void resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int& numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol);

	bool contained(vector<int> ordenDeUso, int cifra, int solMejor);


};

#endif// DFS_H