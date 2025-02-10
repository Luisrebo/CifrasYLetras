#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <vector>
#include "Search.h"

using namespace std;

class Factory {
public:
    // M�todo est�tico que crea el objeto de la clase derivada adecuada
    static unique_ptr<Search> create(const string& tipo,int nObjetivo, vector<int>& nCandidatos);
};

#endif // FACTORY_H
