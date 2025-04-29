#include "HeuristicaFactory.h"

// Todas las que puedo crear
#include "HeuristicaPorAltura.h"
#include "HeuristicaPorPalabrasAlcanzables.h"
#include "HeuristicaPorOrdenAlfabetico.h"
#include "HeuristicaPorOrdenDeProbabilidades.h"

std::unique_ptr<IHeuristica> HeuristicaFactory::crearHeuristica(TipoHeuristica tipo) {

	switch (tipo) {

	case TipoHeuristica::AlFABETICO:
		return std::make_unique<HeuristicaPorAltura>();

	case TipoHeuristica::ALTURA:
		return std::make_unique<HeuristicaPorPalabrasAlcanzables>();

	case TipoHeuristica::PALABRASALCANZABLES:
		return std::make_unique<HeuristicaPorOrdenAlfabetico>();

		/*case TipoHeuristica::PROBABILIDADES:
			 return std::make_unique<>();*/


	default:
		return nullptr; // o lanzar excepción si prefieres
	}
}


//para la de probabilidades.
std::unique_ptr<IHeuristica> HeuristicaFactory::crearHeuristica(TipoHeuristica tipo, const MatricesDeProbabilidades& probs) {

	if (tipo==TipoHeuristica::PROBABILIDADES) {
		return std::make_unique<HeuristicaPorOrdenDeProbabilidades>(probs);
	}
	return crearHeuristica(tipo);
}