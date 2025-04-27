#ifndef PROMEDIO_GLOBAL_H
#define PROMEDIO_GLOBAL_H

#include "FuncionesComunes.h"

/// Envoltura ligera de CollectorLetras para exponer interfaz de ProbabilidadGlobalLetras
class PromedioGlobal {
public:
	PromedioGlobal();

	/// Imprime el resumen global delegando al collector interno
	void imprimirResumen(std::ostream& os = std::cout) const;

	void updateStatsToGlobal(CommonStatsCifrasYLetrasSingleCase &singleCase);

	

private: 
	
	double PromedioNumeroDeNodosVisitados;
	double PromedionumeroDeNodosCompletamenteExplorados;
	double  PromedioNumeroDeVecesActualizaSolucion;
	double  PromedioNivelMaximoAlcanzado;
	double PromedioNivelSolucion;
	int numCasosProcesados;
};

#endif // PROBABILIDAD_GLOBAL_LETRAS_H

