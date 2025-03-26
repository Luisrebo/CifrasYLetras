#include "pruebaCifras.h"
#include "Factory.h"    // Asume que Factory::create está declarado y definido correctamente
#include "BFS.h"
#include "DFS.h"
#include "FuncionesComunes.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

std::string resuelveCasoCifras(std::string mode, const std::vector<int>& input) {
    // Declaramos searchResult usando el constructor predeterminado
    SearchResult searchResult;
    int numObjetivo = input.back();  // Último elemento es el objetivo
    
    // Verificamos que el vector tenga el tamaño esperado: CIFRAS_INICIALES + 1 (último elemento es el objetivo).
    if (input.size() != CIFRAS_INICIALES + 1) {
        return "Input incorrecto";
    }

    
    
    // Copiar las cifras (los primeros CIFRAS_INICIALES elementos) a un std::array
    std::array<int, CIFRAS_INICIALES> numerosCandidatos;
    for (size_t i = 0; i < CIFRAS_INICIALES; ++i) {
        numerosCandidatos[i] = input[i];
    }
    
    // Convertir el array de candidatos al tipo esperado por Factory::create.
    // Se asume que Factory::create está definido para recibir un std::array<int, CIFRAS_INICIALES>&
    auto search = Factory::create(mode, numObjetivo, numerosCandidatos);
    searchResult = search->busqueda();
    
    return formatearSolucion(searchResult);
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

        cout<< "COnsola"<<result._ordendeUso[indiceCifras] << result._operacionesEnOrden[indiceOperaciones] << result._ordendeUso[indiceCifras + 1]
            << "=" << calcular(result._operacionesEnOrden[indiceOperaciones], result._ordendeUso[indiceCifras], result._ordendeUso[indiceCifras + 1])
            << '\n';

		oss << result._ordendeUso[indiceCifras] << result._operacionesEnOrden[indiceOperaciones] << result._ordendeUso[indiceCifras + 1]
			<< "=" << calcular(result._operacionesEnOrden[indiceOperaciones], result._ordendeUso[indiceCifras], result._ordendeUso[indiceCifras + 1])
			<< '\n';
	}
	return oss.str();

}