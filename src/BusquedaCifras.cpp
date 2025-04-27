#include "BusquedaCifras.h"



BusquedaCifras::BusquedaCifras(){}

//leemos y vamos resolviendo caso a caso llamando a solve
void BusquedaCifras::resuelve(std::istream& archivoEntrada){
	int numCasos,numObjetivo;
	
	
	archivoEntrada >> numCasos;
	archivoEntrada >> MODE;

	//leemos cada caso
	for (int i = 0; i < numCasos; ++i) {

		//numero objetivo del caso (solucion a la que aspiramos)
		archivoEntrada >> numObjetivo;

		//vector de cifras que podemos usar
		array<num_t, CIFRAS_INICIALES> numerosCandidatos;

		//leemos y alojamos en el array las cifras
		for (int j = 0; j < CIFRAS_INICIALES; ++j)
			archivoEntrada >> numerosCandidatos[j];

		solve(numObjetivo, MODE, numerosCandidatos);

	}
	promedioGlobal.imprimirResumen();
}

//reslvemos caso a caso
SearchResult BusquedaCifras::solve(int numObjetivo, string MODE,array<num_t, CIFRAS_INICIALES> &numerosCandidatos){

	//creamos el search con la factoria
	auto search = SearchFactory::create(MODE, numObjetivo, numerosCandidatos);

	//stadisticas de un solo caso
	StatsSingleCaseCifras statsSingleCase;

	SearchResult result = search->busqueda(statsSingleCase);

	promedioGlobal.updateStatsToGlobal(statsSingleCase.statsComunes);

	return result;
}
