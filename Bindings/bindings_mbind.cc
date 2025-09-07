#include <emscripten/bind.h>
#include "numbersChallenge.h"
#include "lettersChallenge.h"
#include "commonFunctions.h" 
#include <vector>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(prueba) {
    // Exponer el struct SearchResult a JavaScript
    emscripten::value_object<SearchResult>("SearchResult")
        .field("operacionesEnOrden", &SearchResult::_operacionesEnOrden)
        .field("ordenDeUso", &SearchResult::_ordendeUso)
        .field("mejorSol", &SearchResult::_mejorSol)
        ;

    // Exponer la clase SolucionLetras como una clase para poder usar métodos.
    class_<SolucionLetras>("SolucionLetras")
        .constructor<>()
        .function("getPalabra", &SolucionLetras::getPalabra)
        ;


    
    // Exponer las funciones que devuelven SearchResult
    emscripten::function("resuelveCifras", &resuelveCasoCifras);
    emscripten::function("resuelveLetras", &resuelveCasoLetras);

    // Registra std::vector<int> para que se pueda usar desde JavaScript
    emscripten::register_vector<int>("VectorInt");

    //necesario porque lo usamos en SearchResult 
     emscripten::register_vector<char>("VectorChar");

}
