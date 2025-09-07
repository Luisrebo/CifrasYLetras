#include "HeuristicFactory.h"
// Todas las que puedo crear
#include "HeightHeuristic.h"
#include "ReachableWordsHeuristic.h"
#include "AlphabeticalOrderHeuristic.h"
#include "ProbabilityOrderHeuristic.h"
#include <memory>

std::unique_ptr<IHeuristic> HeuristicFactory::crearHeuristica(TipoHeuristica tipo) {

	switch (tipo) {

	case TipoHeuristica::AlFABETICO:
		return std::make_unique<AlphabeticalOrderHeuristic>();
		

	case TipoHeuristica::ALTURA:
		return std::make_unique<HeightHeuristic>();
		

	case TipoHeuristica::PALABRASALCANZABLES:
		return std::make_unique<ReachableWordsHeuristic>();

		/*case TipoHeuristica::PROBABILIDADES:
			 return std::make_unique<>();*/


	default:
		return nullptr; // o lanzar excepción si prefieres
	}
}


//para la de probabilidades.
std::unique_ptr<IHeuristic> HeuristicFactory::crearHeuristica(TipoHeuristica tipo, const MatricesDeProbabilidades& probs) {

	if (tipo==TipoHeuristica::PROBABILIDADES) {
		return std::make_unique<ProbabilityOrderHeuristic>(probs);
	}
	return crearHeuristica(tipo);
}