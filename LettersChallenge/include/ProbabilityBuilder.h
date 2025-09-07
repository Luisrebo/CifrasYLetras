#ifndef PROBABILITY_BUILDER_H
#define PROBABILITY_BUILDER_H

#include "commonFunctions.h"
#include <array>
#include <string>
#include <utility>

class ProbabilityBuilder {

protected:

	ProbabilidadesParaHeuristica probabilidades;

	std::array<std::pair<int, char>, 27> letterCounts{};

public:

	ProbabilityBuilder();

	void update(const std::string& palabraDelDiccionario);

	void calcularProbabilidades();

	MatricesDeProbabilidades getProbabilidades();


};
#endif // PROBABILITY_BUILDER_H
