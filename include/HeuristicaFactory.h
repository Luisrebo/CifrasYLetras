#ifndef HEURISTICA_FACTORY_H
#define HEURISTICA_FACTORY_H

#include "IHeuristica.h"
#include "FuncionesComunes.h"

class HeuristicaFactory {
public:

    // Crea la heurística indicada por su tipo
    static std::unique_ptr<IHeuristica> crearHeuristica(TipoHeuristica tipo, const MatricesDeProbabilidades& probs);
    // Crea la heurística indicada por su tipo
    static std::unique_ptr<IHeuristica> crearHeuristica(TipoHeuristica tipo);
};



#endif