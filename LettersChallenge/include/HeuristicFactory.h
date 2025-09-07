#ifndef HEURISTIC_FACTORY_H
#define HEURISTIC_FACTORY_H

#include "IHeuristic.h"
#include "commonFunctions.h"
#include <memory>

class HeuristicFactory {
public:

    // Crea la heurística indicada por su tipo
    static std::unique_ptr<IHeuristic> crearHeuristica(TipoHeuristica tipo, const MatricesDeProbabilidades& probs);
    // Crea la heurística indicada por su tipo
    static std::unique_ptr<IHeuristic> crearHeuristica(TipoHeuristica tipo);
};



#endif