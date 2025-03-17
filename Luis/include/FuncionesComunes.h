#ifndef FuncionesComunes_H
#define FuncionesComunes_H


#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;
//2bytes en vez de 4 de int
// -32,768 a 32,767.
using num_t = int;

//mejor que const?
constexpr short CIFRAS_INICIALES = 6; //cifras iniciales/disponibles/operandos
constexpr short CIFRAS_MAXIMAS_ENCADENADAS = 2 * CIFRAS_INICIALES; //maximo numero de cifras que van a ser utilizadas en una solucion (caso peor: caso secuencial :11)
const std::string BFS_MODE = "BFS";
const std::string DFS_MODE = "DFS";

/*PRUEBA DE LETRAS*/
constexpr short NUMERO_DE_LETRAS = 10;

struct Operacion {
	char simbolo;  // el símbolo de la operación
	std::function<num_t(num_t, num_t)> op;  // la operación misma
	std::function<bool(num_t, num_t)> valida;  // ¿cuándo la operación es válida?
};

//estructura comun para devolver el resultado de search y poder tratarlo en clases como Main
struct SearchResult {
	std::vector<char> _operacionesEnOrden;
	std::vector<num_t> _ordendeUso;
	num_t _mejorSol;

	// Constructor predeterminado
	SearchResult() : _mejorSol(0) {
		// Los vectores se inicializan vacíos por defecto
	}

	SearchResult(const std::array<char, CIFRAS_INICIALES>& operacionesEnOrden, const std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS>& _ordendeUso,
		num_t nivel, num_t mejorSol);
};
struct SolucionLetras {
	std::array<char, NUMERO_DE_LETRAS> palabraSolucion;
	short int longitud;

	SolucionLetras() : palabraSolucion{}, longitud(0) {}
};


num_t calcular(char operacion, num_t solucionParcial, num_t operando);

#endif