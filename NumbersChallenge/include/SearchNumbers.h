#ifndef SEARCH_NUMBERS_H
#define SEARCH_NUMBERS_H


#include "commonFunctions.h"
#include "GlobalAverage.h"
#include <istream>   
enum class Mode;


class SearchNumbers {

public:
	SearchNumbers();

	//leemos y vamos resolviendo caso a caso llamando a solve
	void resuelve(std::istream& in);

	//reslvemos caso a caso
	SearchResult solve(num_t numObjetivo, Mode mode,const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos);

private:

	// modo elegido (BFS/DFS)
	Mode mode;
	GlobalAverage promedioGlobal;
};


#endif
