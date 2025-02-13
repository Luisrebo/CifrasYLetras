#ifndef TRIE_H_
#define TRIE_H_

//#include <algorithm>
//#include <functional>
//#include <stack>
//#include <stdexcept>
//#include <utility>
#include <map>
#include <string>
using namespace std;

class Trie {

protected:
	struct TreeNode;
	using Link = TreeNode*;
	struct TreeNode {
		char elem;
		map<char, Link> hijos;// Mejor char que String, mapa de maximo de 27 caracteres (letras del abecedario)
		int altura;
		bool terminal;
		

		TreeNode(char const& e, int alt) : elem(e), altura(alt), terminal(false), hijos() {}
	};
	// puntero a la raíz de la estructura jerárquica de nodos
	Link raiz;

	//un atributo profundidad para que sepamos por donde empezar a buscar algo euleriano que se comunique a la vuelta de la recursiva?

	// número de elementos (cardinal del conjunto)
	int nelems;


public:

	// constructor (conjunto vacío)
	Trie() : raiz(nullptr), nelems(0), {}

	~Trie() {
		libera(raiz);
	};

	bool insert(string const& palabra) {
		for(char letra:palabra)
			if (raiz->hijos.find(letra)) {

			}
		return inserta(palabra, raiz);
	}
protected:
	static void libera(Link a) {
		if (a != nullptr) {

			for (const auto& par : a->hijos) 
				libera(par.second);
			
			delete a;
		}
	}

	bool inserta(string const& e, Link& a) {
		bool crece;
		if (a == nullptr) { // se inserta el nuevo elemento e
			a = new TreeNode(e[0],1); //la raiz es null entonces la altura es 1
			++nelems;
			crece = true;
		}

		//si la letra no existe en este nivel
		else if (a->hijos.find(e[a->altura]) != a->hijos.end()) {//si la clave no existe el map no devuelve null devuelve hijos.end()

			Link nuevoHijo = new  TreeNode(e[a->altura - 1], a->altura+1);

			a->hijos.insert(std::make_pair(e[a->altura - 1], nuevoHijo));//usamos make pair porque no queremos sobreescribir en ningun caso
			++nelems;
			crece = true;

			//tras insertar la primera letra que no teniamos seguiremos insertando las demas
			inserta(e, nuevoHijo);
		}

		else{}
	}
};


#endif