#ifndef TRIE_H_
#define TRIE_H_

#include <map>
#include <string>
#include <queue>
#include <set>

#include "TrieQuery.h"
using namespace std;



class Trie {

protected:
	struct TreeNode;
	using Link = TreeNode*;
	struct TreeNode {
		char elem;
		map<char, Link> hijos;// Mejor char que String, mapa de maximo de 27 caracteres (letras del abecedario)

		/*struct deepComparator {
			// Compara por profundidad primero los mas profundos(mas prometedor)
			bool operator()(const Link& l1, Link& l2) const {
				return l1->profundidad < l2->profundidad;
			}
		};

		priority_queue<Link, vector<Link>, deepComparator> hijosPorProfundidad;*/
		struct deepComparator {
			bool operator()(Link const l1, Link const l2) const {
				if (l1->profundidad == l2->profundidad)
					return l1 < l2; // por que es necesario el deempate?
				return l1->profundidad > l2->profundidad;
			}
		};
	
		set<Link, deepComparator> hijosPorProfundidadSet;

		int altura;//altura del nodo respeccto a la raiz
		bool terminal;//flag para ver si es palabra de nuestro abcedario
		int profundidad;//lo usaremos para podar

		TreeNode(char const& e, int alt, int deep) : elem(e), altura(alt), terminal(false), hijos(), profundidad(deep) {}
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

	//resolver un caso, dada una cadena de letras
	void solve(string const& letras) {
		TrieQuery problema(letras);
		Solucion solParcial;

		//exploramos el trie buscando palabras que contengan las letras de la cadena recibida
		//explorar(raiz, problema, solParcial);
		explorarRapido(raiz, problema, solParcial);
		problema.imprimirSolucion(problema);
	}
	
	void insert(string const& palabra) {

		vector<Link> nodosAAumentarProfundidad;
		//nodo objetivo es el ultimo nodo comun entre la palabra y el trie o la raiz
		//ejem Trie tiene acabariamos y yo inserto acabados search devuelve a
		//a tenia profundidad 6 aunque yo haya insertado 3 letras la profundidad en a se mantiene inserta devolveria 3
		Link nodoObjetivo = search(palabra, raiz, nodosAAumentarProfundidad);


		//cuidado si queremos instertar una cadena que sea subcadena de otra existente el flag terminal no estara a true pero existira su camino
		//Cuidado podemos querer inseratr sol y que exista solo entonces el arbol de sol estara pero sin terminal=true
		//de ser asi al entrar en inserta cambiaremos el terminal del ultimo nodo a true y ya la tendremos representada
		if (!existe(palabra, nodoObjetivo)) {//si la profundida crece (insertamos un camino con mas altura de la que tenia su camino maximo)
			short int profundidadAumentada = inserta(palabra, nodoObjetivo);
			//En este punto y al haber enviado el ultimo nodo comun(nodoObjetivo) a la funcion inserta
			// de ser necesaria su profundidad ya estara actualizada

			//si el ultimo nodo comun era la raiz no queremos actualizar 
			if (nodoObjetivo != raiz)
				actualizarProfundidadTrasInsertar(profundidadAumentada, nodosAAumentarProfundidad);

		}

	}
	void actualizarProfundidadTrasInsertar(short int profundidadAumentada, vector<Link>& nodosAAumentarProfundidad) {

		//En este punto y al haber enviado el ultimo nodo comun(nodoObjetivo) a la funcion inserta
		// de ser necesaria su profundidad ya estara actualizada

		//los nodos estan en el array por si al añadir una palabra hay que actualizar su 
		//profundidad maxima y han sido añadidos segun el camino que se sigio empezando por la raiz hasta el ultimo nodo comun entre la palabra y el trie
		//por lo que los vamos a reccorer en orden lifo, la raiz la ultima para facilitar la actualizacion de profundidad
		//empezaremos a actualizar por el ultimo nodo no comun ya que el ultimo nodo comun entre trie y palabra
		//lo mandamos a insertar y ya se actualizo alli entonces profundidadNodoHijoAnterior es la prof
		//de aquel ultimo nodo comun que enviamos
		// 
		//int profundidadNodoHijoAnterior = nodoObjetivo->profundidad;
		int profundidadNodoHijoAnterior = nodosAAumentarProfundidad[nodosAAumentarProfundidad.size() - 1]->profundidad;
		//en cuanto un nodo no actualice su profundidad ninguno de sus progenitores debera hacerlo
		bool dejarDeActualizarProfundidad = false;
		int i = nodosAAumentarProfundidad.size() - 2;//nos saltamos el ultimo que fue actualizado en inserta(nodoObjetivo) ultimo nodo comun en el trie

		while (i >= 0 && !dejarDeActualizarProfundidad) {
			Link NodoAntecesorAActualizarProfundidad = nodosAAumentarProfundidad[i];

			//si el nodo ya tiene mas profundidad que el hijo actualizado +1 (hijo+el mismo) no habra que seguir
			//actualizando antecesores porque ya lo estaran ya que esta profundidad la tenia por otra ramificacion
			//distinta a la creada por la palabra que acabamos de insertar
			if (NodoAntecesorAActualizarProfundidad->profundidad >= profundidadNodoHijoAnterior + 1)
				dejarDeActualizarProfundidad = true;

			//si es la nueva palabra la que ha generado la mayor profundidad act el nudo progenitor
			else
				NodoAntecesorAActualizarProfundidad->profundidad = profundidadNodoHijoAnterior + 1;

			i--;
			profundidadNodoHijoAnterior = NodoAntecesorAActualizarProfundidad->profundidad;
		}
	}
protected:
	//Buscamos una palabra, devolvemos el nodo que representa el ultimo caracter de la cadena que existe en el trie.
	//funcion que explora las ramificaciones de un nodo y, dada una cadena string devuleve el nodo a partir del cual no existe una correlacion en el trie o devuelve el 
	// nodo que representa el ultimo caracter de la cadema si esa cadena ya estaba representada anteriormente en el trie
	TreeNode* search(string const& palabraBuscada, Link& nodo, vector<Link>& nodosAAumentarProfundidad) { //Ruben me dijo que con tipos pequeños como int no usara& con string si? lo hace alberto en el tad
		if (nodo == nullptr) { //deberia trabajar con node en todo el cuerpo? como lo
			raiz = new TreeNode(' ', 0, 0); //la raiz es null entonces la altura y profundidad es 0 y metemos el caracter vacio
			++nelems;
			return raiz;
		}
		else {
			if (palabraBuscada.length() < nodo->altura)//si la altura es mayor que la longitud de la palabra es porque hemos encontrado la cadena que buscabamos ya que sino habriamos acabado antes
				return nodo;

			//clave valor del map de hijos con el caracter de la palabra en la posicion altura o a->hijos.end() si no existe
			//Para un nodo con altura nodo->altura el nodo que representa el caracter que buscamos sera: palabrabuscada[a->altura] 
			// ya que la raiz tiene altura 0 y no representa ningun caracter, para los hijos de la raiz buscaremos el caracter palabraBuscada[0]
			auto ClaveValorNodoHijo = nodo->hijos.find(palabraBuscada[nodo->altura]);

			//si el nodo no tiene ningun hijo con el caracter que buscamos
			if (ClaveValorNodoHijo == nodo->hijos.end())
				return nodo;//devolvemos el nodo a partir del cual difieren las palabras del trie con la insertada osea el ultimo nodo comun

			else {
				nodosAAumentarProfundidad.push_back(nodo);
				return search(palabraBuscada, ClaveValorNodoHijo->second, nodosAAumentarProfundidad);//seguimos explorando por el hijo que coincida con el caracter }

			}
		}
	}
	//nodo ya es el nodo que representa el ultimo caracter de la cadena que existe en el trie (devuelto por busqueda)
	//devolvemos true si crecemos en profundidad para actualizar la profundidad de los antecesores
	int inserta(string const& palabra, Link& nodo) {

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
			Link nuevoHijo = new TreeNode(palabra[nodo->altura], nodo->altura + 1, 0);
			nodo->hijos.insert(std::make_pair(palabra[nodo->altura], nuevoHijo));//palabra[nodo->altura]=nuevoHijo->elem;
			//añadimos tamb el nuevo hijo al set de prioridad para cuando exploremos soluciones tengamos en cuenta los nodos mas prometedores anttes
			nodo->hijosPorProfundidadSet.insert(nuevoHijo);

			//seguimos insertando nodos y a la vuelta de la recursiva vamos actualizando las profundidades
			nodo->profundidad = max(inserta(palabra, nuevoHijo) + 1, nodo->profundidad);// si en el nodo actual yo ya tenia un hijo mas profundo que el que acabo de expandir
			return nodo->profundidad; //esto no es un poco raro? devolvemos true porque crece el arbol
		}
	}
	
	//exploramos recursivamente los nodos descendentes de node y en cada nodo tratamos las posibles soluciones que generen sus hijos
	void explorar(Link& node, TrieQuery& problema, Solucion& solParcial) {//struct Solucion definida en TrieQuery

		//recorremos los diferentes hijos del nodo actual
		for (auto ParnodoHijo : node->hijos) {

			//consultamos si el hijo que estamos recorriendo tiene un caracter valido y quedan letras de ese caracter sin usar
			auto ParLetraCantidadDisponibles = problema.mapaLetrasDisponibles.find(ParnodoHijo.first);

			//si la letra del nodo hijo que estamos explorando la tenemos en las letras de la prueba y no hemos usado todas las que tenimos y puede haber una sol mejor
			if (ParLetraCantidadDisponibles != problema.mapaLetrasDisponibles.end() && ParLetraCantidadDisponibles->second > 0 && problema.mejorSolucion.longitud < (ParnodoHijo.second->altura + ParnodoHijo.second->profundidad)) {/*Posible poda:&& problema.solMejor->longitud<node.niel+node.profundidad*/

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
	/*Explorar con cola de prioridad en los nodos para explorar por profundidad y tratar de encontrar soluciones mas rapido*/
	void explorarRapido(Link& node, TrieQuery& problema, Solucion& solParcial) {

		//recorremos los diferentes hijos del nodo actual
		for (auto ParnodoHijo : node->hijosPorProfundidadSet) {

			//consultamos si el hijo que estamos recorriendo tiene un caracter valido y quedan letras de ese caracter sin usar
			auto ParLetraCantidadDisponibles = problema.mapaLetrasDisponibles.find(ParnodoHijo->elem);

			//si la letra del nodo hijo que estamos explorando la tenemos en las letras de la prueba y no hemos usado todas las que tenimos y puede haber una sol mejor
			if (ParLetraCantidadDisponibles != problema.mapaLetrasDisponibles.end() && ParLetraCantidadDisponibles->second > 0 && problema.mejorSolucion.longitud < (ParnodoHijo->altura + ParnodoHijo->profundidad)) {/*Posible poda:&& problema.solMejor->longitud<node.niel+node.profundidad*/

				//Marcadores
				//actualizamos solucion parcial y comprobamos si es solucion total
				solParcial.palabraSolucion[node->altura] = ParnodoHijo->elem;
				solParcial.longitud = node->altura + 1;
				ParLetraCantidadDisponibles->second -= 1; //restamos uno a la cantidad de letras disponible con este  caracter 

				//si el nodo hijo forma una palabra de nuestro abecedeario (ya sabemos que su letra esta disponible)
				//y si es de mayor longitud que la mejor palabra que habiamos encontrado
				if (ParnodoHijo->terminal == true && ParnodoHijo->altura > problema.mejorSolucion.longitud)
					problema.mejorSolucion = solParcial;//hacemos una copia de los datos para actualizar la mejor sol

				explorarRapido(ParnodoHijo, problema, solParcial);

				//desmarcamos
				//solParcial.palabraSolucion[node->altura] = '0/';//aporta algo?
				solParcial.longitud = node->altura;
				ParLetraCantidadDisponibles->second += 1; //sumamos uno a la cantidad de letras disponible con este  caracter 


			}
		}
		return;
	
	}

public:
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


};


#endif
