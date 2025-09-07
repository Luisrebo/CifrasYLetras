#include "GlobalAverage.h"
#include <iostream>
#include <iomanip> //para quitar not cientifica set precision(decimales)

GlobalAverage::GlobalAverage()
	: PromedioNumeroDeNodosVisitados(0.0),
	PromedionumeroDeNodosCompletamenteExplorados(0.0),
	PromedioNumeroDeVecesActualizaSolucion(0.0),
	PromedioNivelMaximoAlcanzado(0.0),
	PromedioNivelSolucion(0.0),
	numCasosProcesados(0)
{}

//llamamos a esta funcion tras haber terminado la exploracion de un caso completamente
void GlobalAverage::updateStatsToGlobal(const CommonStatsCifrasYLetrasSingleCase& singleCase) {

	numCasosProcesados += 1;

	//promedio de cuantas veces es actualizada la solucion (para ver como de directo encontramos la soluicion)
	PromedioNumeroDeVecesActualizaSolucion = (PromedioNumeroDeVecesActualizaSolucion * (numCasosProcesados - 1) + singleCase.numeroDeVecesActualizaSolucion) / numCasosProcesados;

	//actualizamos el promedio de nodos visiatdos;
	PromedioNumeroDeNodosVisitados = (PromedioNumeroDeNodosVisitados * (numCasosProcesados - 1) + singleCase.numeroDeNodosVisitados) / numCasosProcesados;

	//promedio de los nodos explorados completamente, todos sus hijos son explorados
	PromedionumeroDeNodosCompletamenteExplorados = (PromedionumeroDeNodosCompletamenteExplorados * (numCasosProcesados - 1) + singleCase.numeroDeNodosCompletamenteExplorados) / numCasosProcesados;

	//promedio de los niveles maximos alcanzables, maximo nivel de profundidad quese llega a visitar
	PromedioNivelMaximoAlcanzado = (PromedioNivelMaximoAlcanzado * (numCasosProcesados - 1) + singleCase.nivelMaximoAlcanzado) / numCasosProcesados;

	//promedio de niveles donde se encontraron soluciones, promedio de las longitudes de las soluciones
	PromedioNivelSolucion = (PromedioNivelSolucion * (numCasosProcesados - 1) + singleCase.nivelSolucion) / numCasosProcesados;
}

void GlobalAverage::imprimirResumen(std::ostream& os) const {

	//resuemn de os promedios de resolver todos los casos de la rueba de letras
	os << std::fixed << std::setprecision(2)
		 << "=== Resumen de promedios (cifras/letras) ===\n"
		 << "Promedio nodos visitados: " << PromedioNumeroDeNodosVisitados << '\n'
		 << "Promedio nodos explorados completamente: " << PromedionumeroDeNodosCompletamenteExplorados << '\n'
		 << "Promedio nivel/longitud de soluciones: " << PromedioNivelSolucion << '\n'
		 << "Promedio actualizaciones de solución: " << PromedioNumeroDeVecesActualizaSolucion << "\n"
		 << "============================================\n";
}
