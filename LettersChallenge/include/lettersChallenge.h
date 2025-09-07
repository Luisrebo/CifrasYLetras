#ifndef LETTERS_CHALLENGE_H
#define LETTERS_CHALLENGE_H

// Es recomendable incluir los headers necesarios para que se reconozcan los tipos.
#include <string>
#include "commonFunctions.h"
#include "Trie.h"

// "using namespace std;" en el header no es lo ideal,
SolucionLetras resuelveCasoLetras(const std::string letras);
std::string formatearSolucionLetras(const SolucionLetras& solucion, std::string letrasDisponibles);


#endif // LETTERS_CHALLENGE_H