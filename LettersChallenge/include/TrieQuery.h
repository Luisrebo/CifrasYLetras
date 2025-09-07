#ifndef TRIE_QUERY_H_
#define TRIE_QUERY_H_

#include <string>
#include "commonFunctions.h"
#include <array>


//Para la prueba de las cifras quiero tener un unico TRIE
//para cada caso de prueba creare un objeto de esta clase para buscar la mejor palabra posible en el trie

//una solucion no es mas que un array de 10 posiciones (cantidad de letras disponibles en la prueba) y una longitud 0<=longitud<=10
class TrieQuery {

public:

	//mejor solucion encontrada para el problema
	SolucionLetras mejorSolucion;
	//mapa con las letras de las que disponibles y la cantidad de ellas que podemos usar (por si tengo varias letras iguales: ¿multiplicidad?)
	//map<char, short int> mapaLetrasDisponibles;
	//array con las letras de las que disponibles y la cantidad de ellas que podemos usar (por si tengo varias letras iguales:
	std::array<int, NUMERO_LETRAS_ABECEDARIO> letrasDisponibleslist;
	std::string letrasDisponibles;


	TrieQuery(const std::string& cadena) :letrasDisponibleslist{}, letrasDisponibles(cadena) {

		//Preparamos el mapa con las letras de las cuales disponemos
		for (char c : letrasDisponibles) {

			//aumntamos en +1 la cantidad de letras de ese tipo que podemos utilizar
			//estando la a en la pos 0 ('a'-'a'=0), la b en la 1 etc
			letrasDisponibleslist[c - 'a'] += 1;

		}

	}
	std::array<char, NUMERO_DE_LETRAS> devolerSolucion() {
		return mejorSolucion.palabraSolucion;
	}
protected:
};
#endif