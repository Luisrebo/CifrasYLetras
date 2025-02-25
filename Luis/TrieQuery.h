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
	//map<char, short int> mapaLetrasDisponibles;
	//array con las letras de las que disponibles y la cantidad de ellas que podemos usar (por si tengo varias letras iguales:
	array< int, 29> letrasDisponibleslist; 
	string letrasDisponibles;


	TrieQuery(string const& cadena) :letrasDisponibles(cadena), letrasDisponibleslist{} {

		//Preparamos el mapa con las letras de las cuales disponemos
		for (char c : letrasDisponibles) {

			//aumntamos en +1 la cantidad de letras de ese tipo que podemos utilizar
			//estando la a en la pos 0 ('a'-'a'=0), la b en la 1 etc
			letrasDisponibleslist[c - 'a'] += 1;

		}

	}
	void imprimirSolucion() {
		if (mejorSolucion.longitud > 0)
			cout << "SOLUCION PARA LAS LETRAS ( ";
		else
			cout << "NO HAY SOLUCION PARA LAS LETRAS: ";

		for (char c : letrasDisponibles)
			cout << c << " ";

		cout << ')' << '\n';

		cout << "Longitud:" << mejorSolucion.longitud << "  ->  ";
		for (int i = 0; i < mejorSolucion.longitud; ++i)
			cout << mejorSolucion.palabraSolucion[i];
		cout << '\n' << '\n';
	}
protected:
};
#endif