#ifndef PRUEBA_CIFRAS_H
#define PRUEBA_CIFRAS_H

// Es recomendable incluir los headers necesarios para que se reconozcan los tipos.
#include <string>
#include <vector>
#include "FuncionesComunes.h"

// "using namespace std;" en el header no es lo ideal,
std::string resuelveCasoCifras(std::string mode, const std::vector<int>& input);

std::string formatearSolucion(const SearchResult &result);

#endif // PRUEBA_CIFRAS_H