#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <array>
#include "Search.h"
#include "FuncionesComunes.h"
using namespace std;

class Factory {
public:
    // Método estático que crea el objeto de la clase derivada adecuada

    static unique_ptr<Search> create(const string& tipo, num_t nObjetivo, array <num_t, CIFRAS_INICIALES>& nCandidatos);
};

#endif // FACTORY_H
