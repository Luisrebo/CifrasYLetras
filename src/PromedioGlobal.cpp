#include "PromedioGlobal.h"
#include <iomanip> //para quitar not cientifica set precision(decimales)

PromedioGlobal::PromedioGlobal()
	: PromedioNumeroDeNodosVisitados(0.0),
	PromedionumeroDeNodosCompletamenteExplorados(0.0),
	PromedioNumeroDeVecesActualizaSolucion(0.0),
	PromedioNivelMaximoAlcanzado(0.0),
	PromedioNivelSolucion(0.0),
	numCasosProcesados(0)
{}

//llamamos a esta funcion tras haber terminado la exploracion de un caso completamente
void PromedioGlobal::updateStatsToGlobal(CommonStatsCifrasYLetrasSingleCase &singleCase) {
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

void PromedioGlobal::imprimirResumen(std::ostream& os) const {

	//resuemn de os promedios de resolver todos los casos de la rueba de letras
	cout << std::fixed << std::setprecision(2)
		<< "=== Resumen Promedioss de los casos  ===\n";
	cout << "Promedio nodos visitados: " << std::fixed << std::setprecision(2) << PromedioNumeroDeNodosVisitados << '\n'
		<< "Promedio nodos explorados Completamente: " << std::fixed << std::setprecision(2) << PromedionumeroDeNodosCompletamenteExplorados << '\n'
		<< "promedio nivel/long de solucioes: " << PromedioNivelSolucion << '\n' << "promedio Actualizacion de soluciones: " << PromedioNumeroDeVecesActualizaSolucion << '\n' << '\n';
	"=======================================\n";
}
