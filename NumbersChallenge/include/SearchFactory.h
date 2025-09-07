#ifndef SEARCH_FACTORY_H
#define SEARCH_FACTORY_H

#include <memory>
#include <array>
#include "Search.h"
#include "commonFunctions.h"

enum class Mode { BFS, DFS };

class SearchFactory {
public:
    // Método estático que crea el objeto de la clase derivada adecuada

    //static unique_ptr<Search> create(const string& tipo, num_t nObjetivo, array <num_t, CIFRAS_INICIALES>& nCandidatos);
    static std::unique_ptr<Search> create(Mode tipo, num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& nCandidatos);
};

#endif // FACTORY_H
