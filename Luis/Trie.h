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
	Trie() : raiz(nullptr), nelems(0) {}

	~Trie() {
		libera(raiz);
	};

	/*bool*/
	void insert(string const& palabra) {
		
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

	//la raiz empieza en altura 0 y todos los hijos cuelgan de ella
	//en cada nivel con altura i estaremos representando el caracter e[i-1] y buscando o insertando un hijo con el caracter el e[i]
	/*bool*/void inserta(string const& e, Link& a) {
		bool crece;

		if (a == nullptr) { // se inserta el nuevo elemento e
			a = new TreeNode(' ', 0); //la raiz es null entonces la altura es 0 y metemos el caracter vacio
			++nelems;
			crece = true;

			inserta(e, a);//seguimos insertando el resto de la palabra
		}
		else {

			//par clave valor del map con first(char id del prefijo) second(Treenode)
			//Guardamos el par del mapa que coincida en este nivel con el char de la posicion/nivel de la palabra que queremos insertar o, sino existe a->hijos.end() 
			auto ParPrefijoNodo = a->hijos.find(e[a->altura]);

			if (a->altura > e.size()) return; //si hemos añadido la ultima letra (o si ya existia un camino de esos prefijos desde la raiz) en altura 1 accedemos a e[0] y en altura size a e[size-1]



			//si la letra no existe en este nivel
			else if (ParPrefijoNodo == a->hijos.end()) {//si la clave no existe el map no devuelve null devuelve hijos.end()

				Link nuevoHijo = new  TreeNode(e[a->altura], a->altura + 1);//cramos nuevo nodo que colgara del anterior, con el elemento=caracter e[altura] porque la raiz epieza con alt 0 y +1 de altura repecto al padre

				a->hijos.insert(std::make_pair(e[a->altura], nuevoHijo));//usamos make pair porque no queremos sobreescribir en ningun momento del programa

				++nelems;
				crece = true;

				//tras insertar la primera letra que no teniamos seguiremos insertando las demas
				inserta(e, nuevoHijo);
			}

			// si existia el prefio en el nivel seguimos con los demas prefijos de la palabra
			else
				inserta(e, ParPrefijoNodo->second); //seguimos insertando a partir del nodo hijo

		}
	}
};


#endif