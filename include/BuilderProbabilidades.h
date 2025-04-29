#ifndef BUILDER_PROBABILIDADES_H
#define BUILDER_PROBABILIDADES_H

#include "FuncionesComunes.h"

class BuilderProbabilidades {

protected:

	ProbabilidadesParaHeuristica probabilidades;

	array<pair<int, char>, 27> leterCounts;

public:

	BuilderProbabilidades();

	void update(const string & palabraDelDiccionario);

	void calcularProbabilidades();

	MatricesDeProbabilidades getProbabilidades();


};


#endif;
