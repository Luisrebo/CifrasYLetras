#include "SearchNumbers.h"
#include "SearchFactory.h"
#include <iostream>   


SearchNumbers::SearchNumbers() : mode(Mode::BFS) {}

//leemos y vamos resolviendo caso a caso llamando a solve
void SearchNumbers::resuelve(std::istream& archivoEntrada){
	int numCasos;
	num_t numObjetivo;
	
	
	archivoEntrada >> numCasos;
	//archivoEntrada >> MODE;

	//leemos cada caso
	for (int i = 0; i < numCasos; ++i) {

		//numero objetivo del caso (solucion a la que aspiramos)
		archivoEntrada >> numObjetivo;

		//vector de cifras que podemos usar
		std::array<num_t, CIFRAS_INICIALES> numerosCandidatos;

		//leemos y alojamos en el array las cifras
		for (int j = 0; j < CIFRAS_INICIALES; ++j)
			archivoEntrada >> numerosCandidatos[j];

		 solve(numObjetivo, Mode::BFS, numerosCandidatos);

	}
	promedioGlobal.imprimirResumen(std::cout);
}

//reslvemos caso a caso
SearchResult SearchNumbers::solve(num_t numObjetivo, Mode mode, const std::array<num_t, CIFRAS_INICIALES> &numerosCandidatos){

	this->mode = mode;

	//creamos el search con la factoria
	auto search = SearchFactory::create(mode, numObjetivo, numerosCandidatos);

	//stadisticas de un solo caso
	StatsSingleCaseCifras statsSingleCase;

	SearchResult result = search->busqueda(statsSingleCase);

	promedioGlobal.updateStatsToGlobal(statsSingleCase.statsComunes);

	return result;
}
