#ifndef BUSQUEDA_CIFRAS_H
#define BUSQUEDA_CIFRAS_H


#include "FuncionesComunes.h"
#include "SearchFactory.h"
#include "PromedioGlobal.h"


class BusquedaCifras {

public:
	BusquedaCifras();

	//leemos y vamos resolviendo caso a caso llamando a solve
	void resuelve(std::istream& in);

	//reslvemos caso a caso
	SearchResult solve(int numObjetivo, string MODE, array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

private:

	//bfs o dfs
	string MODE;
	
	PromedioGlobal promedioGlobal;
};









#endif
