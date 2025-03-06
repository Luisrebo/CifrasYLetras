#include "Factory.h"
#include "BFS.h"
#include "DFS.h"  
#include "FuncionesComunes.h"
#include <memory>
#include <string>
using namespace std;

bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
	if (str1.length() != str2.length()) {
		return false;
	}

	return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
		return std::tolower(a) == std::tolower(b);
		});
}

//unique_ptr<Search> Factory::create(const string& tipo, int nObjetivo,  vector<int>& nCandidatos)
 unique_ptr<Search> Factory::create(const string& tipo, num_t nObjetivo, array<num_t, CIFRAS_INICIALES>& nCandidatos) {
	 
	if (equalsIgnoreCase(tipo, BFS_MODE)) {
		return make_unique<BFS>(nObjetivo, nCandidatos);
	}
	else if (equalsIgnoreCase(tipo, DFS_MODE)) {
		return make_unique<DFS>(nObjetivo, nCandidatos);
	}
	else
		return nullptr;//lanzo excepcion?
}

