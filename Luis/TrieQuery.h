#ifndef TRIE_QUERY_H_
#define TRIE_QUERY_H_

#include <map>
#include <string>

#include "FuncionesComunes.h"
//#include "Trie.h"
using namespace std;

//Para la prueba de las cifras quiero tener un unico TRIE
//para cada caso de prueba creare un objeto de esta clase para buscar la mejor palabra posible en el trie

//una solucion no es mas que un array de 10 posiciones (cantidad de letras disponibles en la prueba) y una longitud 0<=longitud<=10
struct Solucion {
	std::array<char, NUMERO_DE_LETRAS> palabraSolucion;
	short int longitud;

	Solucion(): palabraSolucion{}, longitud(0) {}
};
class TrieQuery {

	public:

	//mejor solucion encontrada para el problema
	Solucion mejorSolucion;
	//mapa con las letras de las que disponibles y la cantidad de ellas que podemos usar (por si tengo varias letras iguales: ¿multiplicidad?)
	map<char, short int> mapaLetrasDisponibles;
	//Trie sobre el cual buscaremos se lo mandamso como parametro? creo que no tiene sentido que lo use al incluir alguna clase?
	//Trie trie;
	//es redundante
	string letrasDisponibles;


	TrieQuery(string const&cadena):letrasDisponibles(cadena){

		//Preparamos el mapa con las letras de las cuales disponemos
		for (char c : letrasDisponibles) {

			auto pairLetraCantidad = mapaLetrasDisponibles.find(c);

			//si no existe la letra la insertamos con cantidad 1
			if (pairLetraCantidad == mapaLetrasDisponibles.end())
				mapaLetrasDisponibles.insert(std::make_pair(c, 1));

			//si ya existia la letra aumentamos la cantidad de veces que podemos usarlaal construir una palabra
			else pairLetraCantidad->second += 1;
		}

	}
	void imprimirSolucion(TrieQuery const& problema) {
		if (problema.mejorSolucion.longitud > 0)
			cout << "SOLUCION PARA LAS LETRAS ( ";
		else
			cout << "NO HAY SOLUCION PARA LAS LETRAS: ";

		for (char c : problema.letrasDisponibles)
			cout << c << " ";

		cout << ')' << '\n';

		cout << "Longitud:" << problema.mejorSolucion.longitud << "  ->  ";
		for (int i = 0; i < problema.mejorSolucion.longitud; ++i)
			cout << problema.mejorSolucion.palabraSolucion[i];
		cout << '\n' << '\n';
	}
protected:
};
#endif