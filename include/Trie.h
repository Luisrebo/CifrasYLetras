#ifndef TRIE_H
#define TRIE_H

#include <string>
#include "TrieQuery.h"
#include "FuncionesComunes.h"

#include <istream>

using namespace std;

class IHeuristica;

class Trie {

public:
	struct TreeNode;
	using Link = TreeNode*;
	struct TreeNode {
		char elem;
		array<Link, NUMERO_LETRAS_ABECEDARIO> hijos;//cambiamos el mapa por un array de 25 elementos (uno por letra)
		int nivel;//distancia en nodos del nodo actual respeccto a la raiz
		bool terminal;//flag para ver si es palabra de nuestro abecedario
		int altura;//dist maxima hasta su hoja mas alejada,lo usaremos para podar
		int numPalabrasAlcanzables; //numero de nodos descendencia con terminal=true, palabras alcanzables, para hacer otra poda
		vector<Link> listaHijosOrdenadosPorHeuristica; //dependiendo la heurstica que hayamos aplicado tendra un orden u otro

		TreeNode(char const& e, int lv) : elem(e), nivel(lv), terminal(false), hijos{}, altura(0), numPalabrasAlcanzables(0) {}
	};


protected:
	// puntero a la raíz de la estructura jerárquica de nodos
	Link raiz;
	std::unique_ptr<IHeuristica> heuristica;

public:
	//SINGLETON
	// Devuelve la única instancia de Trie
	static Trie& getInstance();

	// Carga todas las palabras del diccionario/corpus
	void cargarDesdeArchivo(std::istream& in);

	// Prepara alturas y ordena hijos en todo el trie según la heurística que queramos utilizar para explorar
	void preparar();

	// Resuelve todos los casos llamando uno a uno a solve
	void resuelve(std::istream& in) ;

	//Resuelve cada caso(línea) para tambien llamar de uno en uno en emscriptem
	SolucionLetras solve(std::string letras) const;

	// Cambiamos la heurística que se usamos para ordenar los hijos de cada nodo 
	void setHeuristica(TipoHeuristica tipo);


private:

	Trie();
	~Trie();
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;

	/// Inserta una sola palabra en el trie
	void insert(const std::string& palabra);

	/// Libera memoria recursivamente
	static void libera(Link nodo);

	//nodo ya es el nodo que representa el ultimo caracter de la cadena que existe en el trie (devuelto por busqueda)
	//devolvemos true si crecemos en profundidad para actualizar la profundidad de los antecesores
	int inserta(string const& palabra, Link nodo);

	//Verificamos si una palabra esta representada en el nodo
	//Importamte cotejar terminal puede estar representado su cadena al ser sub cadena de una ya insertada(solo-sol)
	bool existe(string const& palabra, Link const& nodo);

	 int MappingCharToPosition(char c);

	 TreeNode* search(const std::string& palabraBuscada, Link nodo);

};

#endif

