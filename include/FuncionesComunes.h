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
const string BFS_MODE = "BFS";
const string DFS_MODE = "DFS";
const num_t NUMERO_MAXIMO_LETRAS_EN_PALABRA = 10;

/*PRUEBA DE LETRAS*/
constexpr short NUMERO_DE_LETRAS = 10;
constexpr short NUMERO_LETRAS_ABECEDARIO = 27;

//Tipos de heurísticas parala factoria
enum class TipoHeuristica {
	AlFABETICO,
	ALTURA,
	PALABRASALCANZABLES,
	PROBABILIDADES
};

struct CommonStatsCifrasYLetrasSingleCase {
	CommonStatsCifrasYLetrasSingleCase() :
		numeroDeNodosVisitados(0),
		numeroDeNodosCompletamenteExplorados(0),
		numeroDeVecesActualizaSolucion(0),
		nivelMaximoAlcanzado(0),
		nivelSolucion(0),
		numeroDeNodosGenerados(0)
	{}

	int numeroDeNodosGenerados;
	int numeroDeNodosVisitados;
	int numeroDeNodosCompletamenteExplorados;
	int numeroDeVecesActualizaSolucion;

	num_t nivelMaximoAlcanzado;
	num_t nivelSolucion;
};

struct StatsSingleCaseLetras {
	StatsSingleCaseLetras()
		:palabraSolucion{}
	{}

	CommonStatsCifrasYLetrasSingleCase statsComunes;
	std::array<char, NUMERO_DE_LETRAS> palabraSolucion;
};

struct StatsSingleCaseCifras {
	StatsSingleCaseCifras()
		:numeroOperacionesEfectuadas(0)
	{}

	CommonStatsCifrasYLetrasSingleCase statsComunes;
	int numeroOperacionesEfectuadas;
};


//Para la heuristica que recorre los hijos en funcion a sus probabilidades de aparecer, segun las palabras del diccionario/corpus, segun la combinacion de caracter padre e hijo y caracter nivel
using Matriz = vector<vector<double>>;
struct MatricesDeProbabilidades {
	MatricesDeProbabilidades();
	Matriz probabilidadLetraPorNivel; //registramos en cada fila(que representa los niveles del arbol) el numero de veces que aparece una letra (represetado por las columnas)
	Matriz probabilidadLetraPosterior;//matriz cuadrada muestra para cada letra (i) cuantas veces es otra letra(columna) su sucesora
};

struct ProbabilidadesParaHeuristica {
	ProbabilidadesParaHeuristica();

	MatricesDeProbabilidades matricesDeProbabilidades;
	vector<double> sumaTotalDeProbabilidadesPorFilasLetraPorNivel;//lleva la cuenta del total acumulado de las probabilidades de cada fila para hacer las probabilidades celda/total
	vector<double> sumaTotalDeProbabilidadesPorFilasLetraPosterior;

};
struct Operacion {
	char simbolo;  // el símbolo de la operación
	function<num_t(num_t, num_t)> op;  // la operación misma
	function<bool(num_t, num_t)> valida;  // ¿cuándo la operación es válida?
};

//estructura comun para devolver el resultado de search y poder tratarlo en clases como Main
struct SearchResult {
	vector<char> _operacionesEnOrden;
	vector<num_t> _ordendeUso;
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
	string palabraOriginal;

	SolucionLetras() : palabraSolucion{}, longitud(0) {}

	// Método para convertir el array a string 
	std::string getPalabra() const {
		return palabraOriginal;
	}
};


num_t calcular(char operacion, num_t solucionParcial, num_t operando);

#endif