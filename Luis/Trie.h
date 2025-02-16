#ifndef TRIE_H_
#define TRIE_H_

#include <map>
#include <string>

#include "TrieQuery.h"
using namespace std;

class Trie {

protected:
	struct TreeNode;
	using Link = TreeNode*;
	struct TreeNode {
		char elem;
		map<char, Link> hijos;// Mejor char que String, mapa de maximo de 27 caracteres (letras del abecedario)
		int altura;//altura del nodo respeccto a la raiz
		bool terminal;//flag para ver si es palabra de nuestro abcedario
		int profundidad;//lo usaremos para podar

		TreeNode(char const& e, int alt,int deep) : elem(e), altura(alt), terminal(false), hijos(), profundidad(deep){}
	};
	// puntero a la raíz de la estructura jerárquica de nodos
	Link raiz;
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
		
		Link nodoObjetivo=search (palabra, raiz);

		//cuidado si queremos instertar una cadena que sea subcadena de otra existente el flag terminal no estara a true pero existira su camino
		//Cuidado podemos querer inseratr sol y que exista solo entonces el arbol de sol estara pero sin terminal=true
		if(!existe(palabra, nodoObjetivo) )
		inserta(palabra, nodoObjetivo);
	}

	//resolver un caso, dada una cadena de letras
	void solve(string const& letras){
		TrieQuery problema(letras);
		Solucion solParcial;

		//exploramos el trie buscando palabras que contengan las letras de la cadena recibida
		explorar(raiz,problema, solParcial);
		imprimirSolucion(problema);
	}
protected:
	void imprimirSolucion(TrieQuery const& problema) {
		if (problema.mejorSolucion.longitud > 0)
			cout << "SOLUCION PARA LAS LETRAS: ";
		else
			cout << "NO HAY SOLUCION PARA LAS LETRAS: ";

		for (char c : problema.letrasDisponibles)
		cout << c << "  ";

		for (int i = 0; i < problema.mejorSolucion.longitud; ++i)
			cout << problema.mejorSolucion.palabraSolucion[i];
		cout << '\n';
	}
	//exploramos recursivamente los nodos descendentes de node y en cada nodo tratamos las posibles soluciones que generen sus hijos
	void explorar(Link& node, TrieQuery &problema, Solucion &solParcial) {//struct Solucion definida en TrieQuery

		//recorremos los diferentes hijos del nodo actual
		for ( auto ParnodoHijo : node->hijos) {
			
			//consultamos si el hijo que estamos recorriendo tiene un caracter valido y quedan letras de ese caracter sin usar
			auto ParLetraCantidadDisponibles = problema.mapaLetrasDisponibles.find(ParnodoHijo.first);

			//si la letra del nodo hijo que estamos explorando la tenemos en las letras de la prueba y no hemos usado todas las que tenimos y puede haber una sol mejor
			if (ParLetraCantidadDisponibles != problema.mapaLetrasDisponibles.end() && ParLetraCantidadDisponibles->second>0 /* && problema.mejorSolucion.longitud< ParnodoHijo.second->altura + ParnodoHijo.second->profundidad - 1*/) {/*Posible poda:&& problema.solMejor->longitud<node.niel+node.profundidad*/

				//Marcadores
				//actualizamos solucion parcial y comprobamos si es solucion total
				solParcial.palabraSolucion[node->altura] = ParnodoHijo.second->elem;
				solParcial.longitud = node->altura + 1;
				ParLetraCantidadDisponibles->second -= 1; //restamos uno a la cantidad de letras disponible con este  caracter 

				//si el nodo hijo forma una palabra de nuestro abecedeario (ya sabemos que su letra esta disponible)
				//y si es de mayor longitud que la mejor palabra que habiamos encontrado
				if (ParnodoHijo.second->terminal == true && ParnodoHijo.second->altura > problema.mejorSolucion.longitud)
					problema.mejorSolucion = solParcial;//hacemos una copia de los datos para actualizar la mejor sol

				explorar(ParnodoHijo.second, problema, solParcial);

				//desmarcamos
				//solParcial.palabraSolucion[node->altura] = '0/';//aporta algo?
				solParcial.longitud = node->altura;
				ParLetraCantidadDisponibles->second += 1; //sumamos uno a la cantidad de letras disponible con este  caracter 


			}
		}
		return;
	}
	static void libera(Link a) {
		if (a != nullptr) {

			for (const auto& par : a->hijos) 
				libera(par.second);
			
			delete a;
		}
	}

	//Verificamos si una palabra esta representada en el nodo
	//Importamte cotejar terminal puede estar representado su cadena al ser sub cadena de una ya insertada(solo-sol)
	bool existe(string const& palabra, Link const& nodo) {
		return nodo != nullptr && nodo->altura == palabra.size() && nodo->terminal == true;
	}

	//Buscamos una palabra, devolvemos el nodo que representa el ultimo caracter de la cadena que existe en el trie.
	//funcion que explora las ramificaciones de un nodo y, dada una cadena string devuleve el nodo a partir del cual no existe una correlacion en el trie o devuelve el 
	// nodo que representa el ultimo caracter de la cadema si esa cadena ya estaba representada anteriormente en el trie
	TreeNode* search(string const& palabraBuscada, Link& nodo) { //Ruben me dijo que con tipos pequeños como int no usara& con string si? lo hace alberto en el tad
		if (nodo == nullptr) 
			return nullptr;//devuelvo raiz?
		
		else {
			if (palabraBuscada.length() < nodo->altura)//si la altura es mayor que la longitud de la palabra es porque hemos encontrado la cadena que buscabamos ya que sino habriamos acabado antes
				return nodo;

			//clave valor del map de hijos con el caracter de la palabra en la posicion altura o a->hijos.end() si no existe
			//Para un nodo con altura nodo->altura el nodo que representa el caracter que buscamos sera: palabrabuscada[a->altura] 
			// ya que la raiz tiene altura 0 y no representa ningun caracter, para los hijos de la raiz buscaremos el caracter palabraBuscada[0]
			auto ClaveValorNodoHijo = nodo->hijos.find(palabraBuscada[nodo->altura]);

			//si el nodo no tiene ningun hijo con el caracter que buscamos
			if (ClaveValorNodoHijo == nodo->hijos.end())
				return nodo;//devolvemos el nodo a partir del cual difieren las palabras del trie con la insertada

			else
				return search(palabraBuscada, ClaveValorNodoHijo->second);//seguimos explorando por el hijo que coincida con el caracter
		}
	}

	//nodo ya es el nodo que representa el ultimo caracter de la cadena que existe en el trie (devuelto por busqueda)
	//devolvemos true si crecemos en profundidad para actualizar la profundidad de los antecesores
	int inserta(string const& palabra, Link& nodo) {
		//si la raiz es null
		if (nodo == nullptr) { //deberia trabajar con node en todo el cuerpo? como lo
			raiz = new TreeNode(' ', 0,0); //la raiz es null entonces la altura y profundidad es 0 y metemos el caracter vacio
			++nelems;
			
			inserta(palabra, raiz);//seguimos insertando el resto de la palabra
		}
		//vamos a seguir insertando colgando todos los nodos del nodo dado
		//nodo es el ultimo nodo que coincidia con una cadena pudiendo ser tamb el nodo que completa una cadena o un nodo que acabamos de insertar
		else {

			//bool crece;

			//verificamos si ya hemos cabado de insertar la cadena
			// o si la cadena ya estaba insertada pendiente de actualizar el flag "terminal"
			//devolvemos la profundidad que sera 0 si acabamos de insertar un nuevo elemento o palabra.size si la palabra era
			//un subconjunto de una palabra ya existente en el trie
			if (nodo->altura == palabra.size()) {
				nodo->terminal = true;
				return nodo->profundidad;
			}
			else {
				//Creamos un nuevo hijo a partir del padre que represente el primer caracter de la cadena que falte
				Link nuevoHijo=new TreeNode(palabra[nodo->altura],nodo->altura+1,0);
				nodo->hijos.insert(std::make_pair(palabra[nodo->altura], nuevoHijo));//palabra[nodo->altura]=nuevoHijo->elem;
				
				//seguimos insertando nodos y a la vuelta de la recursiva vamos actualizando las profundidades
				 nodo->profundidad=inserta(palabra, nuevoHijo)+1;
				return nodo->profundidad; //esto no es un poco raro? devolvemos true porque crece el arbol
			}
		}
	}
};


#endif
//la raiz empieza en altura 0 y todos los hijos cuelgan de ella
	//en cada nivel con altura i estaremos representando el caracter e[i-1] y buscando o insertando un hijo con el caracter el e[i]
	
/*void inserta(string const& e, Link& a) {
		//bool crece;

		if (a == nullptr) { // se inserta el nuevo elemento e
			a = new TreeNode(' ', 0); //la raiz es null entonces la altura es 0 y metemos el caracter vacio
			++nelems;
			//crece = true;

			inserta(e, a);//seguimos insertando el resto de la palabra
		}
		else {

			//par clave valor del map con first(char id del prefijo) second(Treenode)
			//Guardamos el par del mapa que coincida en este nivel con el char de la posicion/nivel de la palabra que queremos insertar o, sino existe a->hijos.end() 
			auto ParPrefijoNodo = a->hijos.find(e[a->altura]);

			if (a->altura > e.size()) {//si hemos añadido la ultima letra (o si ya existia un camino de esos prefijos desde la raiz) en altura 1 accedemos a e[0] y en altura size a e[size-1]
				a->terminal = true; //es una palabra de nuestro abecedario
				return;
			}

			//si la letra no existe en este nivel
			else if (ParPrefijoNodo == a->hijos.end()) {//si la clave no existe el map no devuelve null devuelve hijos.end()

				Link nuevoHijo = new  TreeNode(e[a->altura], a->altura + 1);//cramos nuevo nodo que colgara del anterior, con el elemento=caracter e[altura] porque la raiz epieza con alt 0 y +1 de altura repecto al padre

				a->hijos.insert(std::make_pair(e[a->altura], nuevoHijo));//usamos make pair porque no queremos sobreescribir en ningun momento del programa

				++nelems;
				//crece = true;

				//tras insertar la primera letra que no teniamos seguiremos insertando las demas
				inserta(e, nuevoHijo);
			}

			// si existia el prefio en el nivel seguimos con los demas prefijos de la palabra
			else
				inserta(e, ParPrefijoNodo->second); //seguimos insertando a partir del nodo hijo

		}
	}*/