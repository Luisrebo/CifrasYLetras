#include <emscripten/bind.h>

#include "pruebaCifras.h"
#include "pruebaLetras.h"


EMSCRIPTEN_BINDINGS(prueba) {
	emscripten::function("resuelveCifras", &resuelveCasoCifras);

	emscripten::function("resuelveLetras", &resuelveCasoLetras);

	// Registra std::vector<int> para que se pueda usar desde JavaScript
	/*Con register_vector<int>("VectorInt") se expone el tipo std::vector<int>, de modo que en JS puedas crear y manipular vectores de enteros mediante new Module.VectorInt().*/
	emscripten::register_vector<int>("VectorInt");
}
