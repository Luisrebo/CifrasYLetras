#include "numbersChallenge.h"
#include "SearchNumbers.h"
#include "commonFunctions.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "SearchFactory.h"
#include <array>

using namespace std;

SearchResult resuelveCasoCifras(std::string mode, const std::vector<int>& input) {
    // Declaramos searchResult usando el constructor predeterminado
    SearchResult searchResult;
    int numObjetivo = input.back();  // Último elemento es el objetivo
    
    // Copiar las cifras (los primeros CIFRAS_INICIALES elementos) a un std::array
    std::array<int, CIFRAS_INICIALES> numerosCandidatos;
    for (size_t i = 0; i < CIFRAS_INICIALES; ++i) {
        numerosCandidatos[i] = input[i];
    }
    
    // Convertir el array de candidatos al tipo esperado por Factory::create.
    // Se asume que Factory::create está definido para recibir un std::array<int, CIFRAS_INICIALES>&
    
    SearchNumbers busquedaCifras;
 
    Mode m = (mode == "BFS" || mode == "bfs") ? Mode::BFS : Mode::DFS;
    return busquedaCifras.solve(numObjetivo, m, numerosCandidatos);
}

std::string formatearSolucion(const SearchResult&result) {
	//mejor que char* porque me ahorro malloc y calculos de memoria, destructres etc
	//va a tamaño justo. #include <sstream>
	ostringstream oss;

	if (result._ordendeUso.empty() || result._operacionesEnOrden.empty())
		return "error";

	//basta con un indice ya que siempre es el doble pero para mayor claridad
	for (int indiceCifras = 0, indiceOperaciones = 0; indiceCifras < result._ordendeUso.size() &&
		indiceOperaciones < result._operacionesEnOrden.size(); indiceCifras += 2, ++indiceOperaciones) {

        /*cout << "COnsola" << result._ordendeUso[indiceCifras] << result._operacionesEnOrden[indiceOperaciones] << result._ordendeUso[indiceCifras + 1]
            << "=" << calcular(result._operacionesEnOrden[indiceOperaciones], result._ordendeUso[indiceCifras], result._ordendeUso[indiceCifras + 1])
            << '\n';*/

		oss << result._ordendeUso[indiceCifras] << result._operacionesEnOrden[indiceOperaciones] << result._ordendeUso[indiceCifras + 1]
			<< "=" << calcular(result._operacionesEnOrden[indiceOperaciones], result._ordendeUso[indiceCifras], result._ordendeUso[indiceCifras + 1])
			<< '\n';
	}
	return oss.str();

}