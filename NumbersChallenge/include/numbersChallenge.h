#ifndef NUMBERS_CHALLENGE_H
#define NUMBERS_CHALLENGE_H

// Es recomendable incluir los headers necesarios para que se reconozcan los tipos.
#include <string>
#include <vector>
#include "commonFunctions.h"

SearchResult resuelveCasoCifras(std::string mode, const std::vector<int>& input);

std::string formatearSolucion(const SearchResult &result);

#endif // NUMBERS_CHALLENGE_H