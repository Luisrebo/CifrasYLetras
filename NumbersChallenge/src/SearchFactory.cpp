#include "SearchFactory.h"
#include "BFS.h"
#include "DFS.h"  
#include <memory>


bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
	if (str1.length() != str2.length()) {
		return false;
	}

	return std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
		return std::tolower(a) == std::tolower(b);
		});
}

//unique_ptr<Search> Factory::create(const string& tipo, int nObjetivo,  vector<int>& nCandidatos)
std::unique_ptr<Search> SearchFactory::create(Mode tipo, num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& nCandidatos) {
	switch (tipo) {
	case Mode::BFS:
		return std::make_unique<BFS>(nObjetivo, nCandidatos);
	case Mode::DFS:
		return std::make_unique<DFS>(nObjetivo, nCandidatos);
	default:
		return nullptr;

	}

}