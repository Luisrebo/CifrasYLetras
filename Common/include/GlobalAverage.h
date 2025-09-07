#ifndef GLOBAL_AVERAGE_H
#define GLOBAL_AVERAGE_H

#include "commonFunctions.h"
#include <iosfwd> // std::ostream
#include <iostream> 


class GlobalAverage {
public:
	GlobalAverage();

	/// Imprime el resumen global delegando al collector interno
	void imprimirResumen(std::ostream& os) const;

	void updateStatsToGlobal(const CommonStatsCifrasYLetrasSingleCase& singleCase);

	

private: 
	
	double PromedioNumeroDeNodosVisitados;
	double PromedionumeroDeNodosCompletamenteExplorados;
	double  PromedioNumeroDeVecesActualizaSolucion;
	double  PromedioNivelMaximoAlcanzado;
	double PromedioNivelSolucion;
	int numCasosProcesados;
};

#endif //GLOBAL_AVERAGE_H

