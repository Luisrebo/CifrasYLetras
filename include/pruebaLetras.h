#ifndef PRUEBA_LETRAS_H
#define PRUEBA_LETRAS_H

// Es recomendable incluir los headers necesarios para que se reconozcan los tipos.
#include <string>
#include <vector>
#include <array>
#include "FuncionesComunes.h"
#include "Trie.h"

// "using namespace std;" en el header no es lo ideal,
std::string resuelveCasoLetras(std::string letras);
std::string formatearSolucionLetras(const SolucionLetras& solucion, std::string letrasDisponibles);
void loadDiccionario(Trie& trie, std::istream& archivo);

#endif // PRUEBA_LETRAS_H