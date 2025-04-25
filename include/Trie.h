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
		array<Link, 29> hijos;//cambiamos el mapa por un array de 25 elementos (uno por letra)
		int nivel;//distancia en nodos del nodo actual respeccto a la raiz
		bool terminal;//flag para ver si es palabra de nuestro abecedario
		int altura;//dist maxima hasta su hoja mas alejada,lo usaremos para podar
		int numPalabrasAlcanzables; //numero de nodos descendencia con terminal=true, palabras alcanzables, para hacer otra poda
		vector<Link> vectorHijosOrdenadoPorPalabrasAlcanzables;//array en el cual realizaremos una copia del arrayy de hijos y ordenaremos para explorar los noodos con mas palabras alcanzables primero
		vector<Link> vectorHijosOrdenadoPorAltura;
		

		//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas altura , mas profundos
		struct highComparator {
			bool operator()(Link const l1, Link const l2) const {
				if (l1->altura == l2->altura)
					return l1 < l2; // por que es necesario el deempate?
				return l1->altura > l2->altura;
			}
		};

		//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas numero de palabras
		struct ReachableWordComparator {
			bool operator()(Link const l1, Link const l2) const {
				if (l1->numPalabrasAlcanzables == l2->numPalabrasAlcanzables)
					return l1 < l2; // por que es necesario el deempate?
				return l1->numPalabrasAlcanzables > l2->numPalabrasAlcanzables;
			}
		};



		TreeNode(char const& e, int lv) : elem(e), nivel(lv), terminal(false), hijos{}, altura(0), numPalabrasAlcanzables(0) {}
	};
	//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas numero de palabras
	
	// puntero a la raíz de la estructura jerárquica de nodos
	Link raiz;
	// número de elementos (cardinal del conjunto)
	int nelems;
	int depurar;
	
public:
	//SINGLETON
	// Devuelve la única instancia de Trie
	static Trie& getInstance() {
		static Trie instance; // Se crea una única vez (lazy initialization) llama al constructor privado una sola vez
		return instance;
	}

	// Elimina el constructor de copia y el operador de asignación
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;


	//resolver un caso, dada una cadena de letras
	SolucionLetras  solve(string const& letras) {

		

		TrieQuery problema(letras);
		SolucionLetras solParcial;

		//Tipos de búsqueda dependiendo de la prioridad de exploracion:
		//¿Que nodos queremos explorar primero?
		//explorarOrdenAlfabetico(raiz, problema, solParcial, statsOneWord);
		//explorarPorNumeroPalabras(raiz, problema, solParcial,statsOneWord);
		explorarPorAltura(raiz, problema, solParcial);
		//explorarOrdenHeuristica(raiz, problema, solParcial, statsOneWord);

		
		//problema.imprimirSolucion();
		//problema.imprimirSolucionParaTest();
		//problema.imprimirPalabrasSolucionParaTest();
		return problema.mejorSolucion;

	}

	void insert(string const& palabra) {
		//Actualizamos las estadistasp para proceder por probabilidades;
		//actualizarProbabilidadesEnCarga(palabra);
		

		//Tipos de búsqueda dependiendo de la prioridad de exploracion:
		//¿Que nodos queremos explorar primero?
		//explorarOrdenAlfabetico(raiz, problema, solParcial, statsOneWord);
		//explorarPorNumeroPalabras(raiz, problema, solParcial,statsOneWord);
		//explorarPorAltura(raiz, problema, solParcial, statsOneWord);
	

		//nodo objetivo es el ultimo nodo comun entre la palabra y el trie o la raiz
		//ejem Trie tiene acabariamos y yo inserto acabados search devuelve a
		//a tenia profundidad 6 aunque yo haya insertado 3 letras la profundidad en a se mantiene inserta devolveria 3
		Link nodoObjetivo = search(palabra, raiz);

		//cuidado si queremos instertar una cadena que sea subcadena de otra existente el flag terminal no estara a true pero existira su camino
		//Cuidado podemos querer inseratr sol y que exista solo entonces el arbol de sol estara pero sin terminal=true
		//de ser asi al entrar en inserta cambiaremos el terminal del ultimo nodo a true y ya la tendremos representada
		if (!existe(palabra, nodoObjetivo)) {//si la profundida crece (insertamos un camino con mas altura de la que tenia su camino maximo)
			inserta(palabra, nodoObjetivo);
			//En este punto y al haber enviado el ultimo nodo comun(nodoObjetivo) a la funcion inserta
			// de ser necesaria su profundidad ya estara actualizada  
		}

	}

protected:

	//Buscamos una palabra, devolvemos el nodo que representa el ultimo caracter de la cadena que existe en el trie.
	//funcion que explora las ramificaciones de un nodo y, dada una cadena string devuleve el nodo a partir del cual no existe una correlacion en el trie o devuelve el 
	// nodo que representa el ultimo caracter de la cadema si esa cadena ya estaba representada anteriormente en el trie
	TreeNode* search(string const& palabraBuscada, Link& nodo) { //Ruben me dijo que con tipos pequeños como int no usara& con string si? lo hace alberto en el tad
		if (nodo == nullptr) { //deberia trabajar con node en todo el cuerpo? como lo
			raiz = new TreeNode(' ', 0); //la raiz es null entonces la altura y profundidad es 0 y metemos el caracter vacio
			++nelems;
			return raiz;
		}
		else {
			if (palabraBuscada.size() == nodo->nivel)//si la altura es mayor que la longitud de la palabra es porque hemos encontrado la cadena que buscabamos ya que sino habriamos acabado antes
				return nodo;

			//cout << palabraBuscada << endl;
			//vamos a ver si el nodo actual tiene un hijo que represente la siguiente letra de la palabra, para ellos vamos a ver si tiene un
			//Link no nulo en la posicion de su array de hijos que corresponda segun el codigo ascii del caracter de la letra que buscamos 
			//representando a la a en la pos 0 y a la z en la 25 (la ñ en la pos 26 que corresponde con ('{' - 'a')
			Link NodoHijo = nodo->hijos[MappingCharToPosition(palabraBuscada[nodo->nivel])];


			//si el nodo no tiene ningun hijo con el caracter que buscamos, su posicion del array esta vacia
			if (NodoHijo == nullptr)
				return nodo;//devolvemos el nodo a partir del cual difieren las palabras del trie con la insertada osea el ultimo nodo comun

			else {

				return search(palabraBuscada, NodoHijo);//seguimos explorando por el hijo que coincida con el caracter 

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
		if (nodo->nivel == palabra.size()) {
			nodo->terminal = true;
			return nodo->altura;
		}
		else {
			//Creamos un nuevo hijo a partir del padre que represente el primer caracter de la cadena que falte
			Link nuevoHijo = new TreeNode(palabra[nodo->nivel], nodo->nivel + 1);
			//añadimos al array de hijos, en la posicion que corresponda segun su ascii al nuevo hijo
			nodo->hijos[MappingCharToPosition(palabra[nodo->nivel])] = nuevoHijo;

			//seguimos insertando nodos y a la vuelta de la recursiva vamos actualizando las profundidades
			nodo->altura = max(inserta(palabra, nuevoHijo) + 1, nodo->altura);// si en el nodo actual yo ya tenia un hijo mas profundo que el que acabo de expandir
			return nodo->altura; //esto no es un poco raro? devolvemos true porque crece el arbol
		}
	}


	//exploramos los hijos dando prioridad por el numero de palabras
	void explorarPorNumeroPalabras(Link& nodo, TrieQuery& problema, SolucionLetras& solParcial) {

		if (problema.mejorSolucion.longitud == 10)
			return;

		

		//recorremos los hijos dando prioridad a los que mas palabras puedan formarse a partir de el 
		for (Link nodoHijo : nodo->vectorHijosOrdenadoPorPalabrasAlcanzables) {

			if (nodoHijo != nullptr) {
				if (problema.mejorSolucion.longitud == 10)
					return;

				//si en el input tenemos la letra que representa el nodo y no la hemos agotado en nieveles superiores 
				//tamb hacemos una poda en la que si la longitud de la solucion parcial que llevamos + la longitud maxima alcanzable para la palabra mas larga que podriamos
				//llegar a obtener es aun asi peor que la mejor sol que ya tenemos saltamos este nodo y exploramos el siguiente
				if (problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] > 0 && problema.mejorSolucion.longitud < (nodoHijo->nivel + nodoHijo->altura)) {

					//marcadores
					solParcial.palabraSolucion[nodo->nivel] = nodoHijo->elem;
					solParcial.longitud = nodo->nivel + 1;
					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] -= 1;

					//si tenemos una solucion mas larga y es una palabra de nuestro vocabulario
					if (/*solParcial.longitud > problema.mejorSolucion.longitud*/ nodoHijo->nivel > problema.mejorSolucion.longitud && nodoHijo->terminal) {
						problema.mejorSolucion = solParcial;

						
						if (problema.mejorSolucion.longitud == 10)
							return;
					}
					explorarPorNumeroPalabras(nodoHijo, problema, solParcial);

					//desmarcamos
					//solParcial.palabraSolucion[nodo->nivel] = '0/';
					//solParcial.longitud -= 1;
					solParcial.longitud = nodo->nivel;

					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] += 1;

				}
			}

		}
		
		return;
	}

	//exploramos recursivamente los nodos descendentes de node y en cada nodo tratamos las posibles soluciones que generen sus hijos
	

	//exploramos recursivamente los nodos descendentes de node y en cada nodo tratamos las posibles soluciones que generen sus hijos
	void explorarOrdenAlfabetico(Link& node, TrieQuery& problema, SolucionLetras& solParcial) {//struct Solucion definida en TrieQuery

		if (problema.mejorSolucion.longitud == 10)
			return;

		

		//recorremos los diferentes hijos del nodo actual
		for (Link nodoHijo : node->hijos) {

			if (nodoHijo != nullptr) {
				if (problema.mejorSolucion.longitud == 10)
					return;
				//consultamos si el hijo que estamos recorriendo tiene un caracter valido y quedan letras de ese caracter sin usar
				//si la letra del nodo hijo que estamos explorando la tenemos en las letras de la prueba y no hemos usado todas las que tenimos y puede haber una sol mejor
				if (problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] > 0 && problema.mejorSolucion.longitud < (nodoHijo->nivel + nodoHijo->altura)) {/*Posible poda:&& problema.solMejor->longitud<node.niel+node.profundidad*/

					//Marcadores
					//actualizamos solucion parcial y comprobamos si es solucion total
					solParcial.palabraSolucion[node->nivel] = nodoHijo->elem;
					solParcial.longitud = node->nivel + 1;
					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] -= 1; //restamos uno a la cantidad de letras disponible con este  caracter 

					//si el nodo hijo forma una palabra de nuestro abecedeario (ya sabemos que su letra esta disponible)
					//y si es de mayor longitud que la mejor palabra que habiamos encontrado
					if (nodoHijo->terminal == true && nodoHijo->nivel > problema.mejorSolucion.longitud) {
						problema.mejorSolucion = solParcial;//hacemos una copia de los datos para actualizar la mejor sol
						
						if (problema.mejorSolucion.longitud == 10)
							return;

					}

					explorarOrdenAlfabetico(nodoHijo, problema, solParcial);

					//desmarcamos
					//solParcial.palabraSolucion[node->altura] = '0/';//aporta algo?
					solParcial.longitud = node->nivel;
					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] += 1; //sumamos uno a la cantidad de letras disponible con este  caracter 


				}
			}
		}

		return;
	}
	//exploramos recursivamente los nodos priorizando los que mas profundos sean
	void explorarPorAltura(Link& node, TrieQuery& problema, SolucionLetras& solParcial) {//struct Solucion definida en TrieQuery

		if (problema.mejorSolucion.longitud == 10)
			return;

		


		//recorremos los diferentes hijos del nodo actual
		for (Link nodoHijo : node->vectorHijosOrdenadoPorAltura) {

			if (nodoHijo != nullptr) {
				if (problema.mejorSolucion.longitud == 10)
					return;
				//consultamos si el hijo que estamos recorriendo tiene un caracter valido y quedan letras de ese caracter sin usar
				//si la letra del nodo hijo que estamos explorando la tenemos en las letras de la prueba y no hemos usado todas las que tenimos y puede haber una sol mejor
				if (problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] > 0 && problema.mejorSolucion.longitud < (nodoHijo->nivel + nodoHijo->altura)) {/*Posible poda:&& problema.solMejor->longitud<node.niel+node.profundidad*/

					//Marcadores
					//actualizamos solucion parcial y comprobamos si es solucion total
					solParcial.palabraSolucion[node->nivel] = nodoHijo->elem;
					solParcial.longitud = node->nivel + 1;
					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] -= 1; //restamos uno a la cantidad de letras disponible con este  caracter 

					//si el nodo hijo forma una palabra de nuestro abecedeario (ya sabemos que su letra esta disponible)
					//y si es de mayor longitud que la mejor palabra que habiamos encontrado
					if (nodoHijo->terminal == true && nodoHijo->nivel > problema.mejorSolucion.longitud) {
						problema.mejorSolucion = solParcial;//hacemos una copia de los datos para actualizar la mejor sol

						

						if (problema.mejorSolucion.longitud == 10)
							return;
					}
					explorarPorAltura(nodoHijo, problema, solParcial);

					//desmarcamos
					//solParcial.palabraSolucion[node->altura] = '0/';//aporta algo?
					solParcial.longitud = node->nivel;
					problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] += 1; //sumamos uno a la cantidad de letras disponible con este  caracter 


				}
			}
		}
		
		return;
	}

	//en codigo ASCII las letras van de 97(a) - 122(z) 
	//en el diccionario vamos a usar el caracter 123({) para representar a la ñ
	//de esta manera vamos a remplazar el map por un arrray de 25 posiciones indexado por el codig ascii de la letra -'a'
	//asignando la pos 0 a la letra a: 'a'-'a'=0 'b'-'a'=1... 
	int MappingCharToPosition(char c) {
		if (c == '-')return 27;

		return c - 'a';
	}


	//En esta funcio recorremos el trie recursivamente y vamos actualizando el vector arrayHijosOrdenadoPorPalabrasAlcanzables para poder hacer una busqueda posteriormente
	//dando prioridad a los nodos con mas palabras alcanzables
	void updateVectorsRecheableWords(Link& link) {

		if (link == NULL)
			return;

		link->vectorHijosOrdenadoPorPalabrasAlcanzables.clear();
		link->vectorHijosOrdenadoPorPalabrasAlcanzables.reserve(29);//para no reasignar capacidad

		for (Link hijo : link->hijos) {
			if (hijo != NULL) {

				link->vectorHijosOrdenadoPorPalabrasAlcanzables.push_back(hijo);
			}
		}
		sort(link->vectorHijosOrdenadoPorPalabrasAlcanzables.begin(), link->vectorHijosOrdenadoPorPalabrasAlcanzables.end(), TreeNode::ReachableWordComparator{});

		for (Link hijo : link->hijos)
			updateVectorsRecheableWords(hijo);
	}
	void updateVectorsHeight(Link& link) {

		if (link == NULL)
			return;

		link->vectorHijosOrdenadoPorAltura.clear();
		link->vectorHijosOrdenadoPorAltura.reserve(29);//para no reasignar capacidad

		for (Link hijo : link->hijos) {
			if (hijo != NULL) {

				link->vectorHijosOrdenadoPorAltura.push_back(hijo);
			}
		}
		sort(link->vectorHijosOrdenadoPorAltura.begin(), link->vectorHijosOrdenadoPorAltura.end(), TreeNode::highComparator{});

		for (Link hijo : link->hijos)
			updateVectorsHeight(hijo);
	}

	int updateHeight(Link& nodo) {
		if (!nodo)//si es null salimos
			return 0;

		int alturaMaxima = 0;
		for (Link hijo : nodo->hijos) {
			if (hijo)
				alturaMaxima = max(alturaMaxima, updateHeight(hijo) + 1);
		}
		nodo->altura = alturaMaxima;
		return nodo->altura;

	}
	
	int updateRecheableWords(Link& nodo) {
		if (!nodo)
			return 0;

		//si el nodo es terminal consideramos que esa palabra es alcanzable desde el ultimo nodo que la representa
		int palabrasMaximasAlcanzables = nodo->terminal ? 1 : 0;

		for (Link hijo : nodo->hijos)
			if (hijo)
				palabrasMaximasAlcanzables += updateRecheableWords(hijo);

		nodo->numPalabrasAlcanzables = palabrasMaximasAlcanzables;
		return palabrasMaximasAlcanzables;
	}

public:
	static void libera(Link a) {
		if (a != nullptr) {

			for (const Link& nodo : a->hijos)
				libera(nodo);

			delete a;
		}
	}
	//vamos a recorrer el trie actualizando el array de hijos alcanzables para poder explorar dando prioridad a los hijos con mas palabras alcanzables
	void updateNodosPodaTrasCargarDiccionary() {
		
		
		updateRecheableWords(raiz);
		updateVectorsRecheableWords(raiz);

		//necesario para buscar por altura y para las podas de todas las busquedas
		updateHeight(raiz);
		updateVectorsHeight(raiz);
	}

	//Verificamos si una palabra esta representada en el nodo
	//Importamte cotejar terminal puede estar representado su cadena al ser sub cadena de una ya insertada(solo-sol)
	bool existe(string const& palabra, Link const& nodo) {
		return nodo != nullptr && nodo->nivel == palabra.size() && nodo->terminal == true;
	}

	
private:
	//SINGLETON
	// constructor (conjunto vacío)
	//El constructor privado se llama la primera vez que se invoca el método estático getInstance().
	//En la linea static Trie instance;
	Trie() : raiz(nullptr), nelems(0), depurar(0) {}

	~Trie() {
		libera(raiz);
	};

};



#endif
