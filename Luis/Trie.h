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

		struct highComparator {
			bool operator()(Link const l1, Link const l2) const {
				if (l1->altura == l2->altura)
					return l1 < l2; // por que es necesario el deempate?
				return l1->altura > l2->altura;
			}
		};
		set<Link, highComparator> hijosPorAlturaSet;

		//ordenamos en el set los nodos dandole mas prioridad a aquellos que tienen mas numero de palabras
		struct ReachableWordComparator {
			bool operator()(Link const l1, Link const l2) const {
				if (l1->numPalabrasAlcanzables == l2->numPalabrasAlcanzables)
					return l1 < l2; // por que es necesario el deempate?
				return l1->numPalabrasAlcanzables > l2->numPalabrasAlcanzables;
			}
		};
		set<Link, ReachableWordComparator> hijosPorPalabrasAlcanzablesSet;

		TreeNode(char const& e, int lv) : elem(e), nivel(lv), terminal(false), hijos{}, altura(0), numPalabrasAlcanzables(0) {}
	};
	// puntero a la raíz de la estructura jerárquica de nodos
	Link raiz;
	// número de elementos (cardinal del conjunto)
	int nelems;
	int depurar;

public:

	// constructor (conjunto vacío)
	Trie() : raiz(nullptr), nelems(0), depurar(0) {}

	~Trie() {
		libera(raiz);
	};

	//resolver un caso, dada una cadena de letras
	void solve(string const& letras) {
		TrieQuery problema(letras);
		SolucionLetras solParcial;

		//exploramos el trie buscando palabras que contengan las letras de la cadena recibida
		//explorar(raiz, problema, solParcial);
		//explorarRapido(raiz, problema, solParcial);
		//explorarRapidoPorPalabras(raiz, problema, solParcial);
		
		explorarPorNumeroPalabras(raiz, problema, solParcial);
		problema.imprimirSolucion();

	}

	void insert(string const& palabra) {
		
		vector<Link> nodosExistentesVisitadosAlInsertar;
		//nodo objetivo es el ultimo nodo comun entre la palabra y el trie o la raiz
		//ejem Trie tiene acabariamos y yo inserto acabados search devuelve a
		//a tenia profundidad 6 aunque yo haya insertado 3 letras la profundidad en a se mantiene inserta devolveria 3
		Link nodoObjetivo = search(palabra, raiz, nodosExistentesVisitadosAlInsertar);


		//cuidado si queremos instertar una cadena que sea subcadena de otra existente el flag terminal no estara a true pero existira su camino
		//Cuidado podemos querer inseratr sol y que exista solo entonces el arbol de sol estara pero sin terminal=true
		//de ser asi al entrar en inserta cambiaremos el terminal del ultimo nodo a true y ya la tendremos representada
		if (!existe(palabra, nodoObjetivo)) {//si la profundida crece (insertamos un camino con mas altura de la que tenia su camino maximo)
			short int profundidadAumentada = inserta(palabra, nodoObjetivo);
			//En este punto y al haber enviado el ultimo nodo comun(nodoObjetivo) a la funcion inserta
			// de ser necesaria su profundidad ya estara actualizada

			//si el ultimo nodo comun era la raiz no queremos actualizar 
			if (nodoObjetivo != raiz) {
				actualizarProfundidadTrasInsertar(profundidadAumentada, nodosExistentesVisitadosAlInsertar);
				actualizarPalabrasAlcanzables(nodosExistentesVisitadosAlInsertar);
			}

		}

	}

protected:
	//Actualizamos con +1 las palabras alcanzables desde los nodos por los cuales transitamos al insertar una nueva palabra
	void actualizarPalabrasAlcanzables(vector<Link>& nodosVisitadosAlInsertar) {
		for (auto link : nodosVisitadosAlInsertar)
			link->numPalabrasAlcanzables += 1;
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
		int profundidadNodoHijoAnterior = nodosAAumentarProfundidad[nodosAAumentarProfundidad.size() - 1]->altura;
		//en cuanto un nodo no actualice su profundidad ninguno de sus progenitores debera hacerlo
		bool dejarDeActualizarProfundidad = false;
		int i = nodosAAumentarProfundidad.size() - 2;//nos saltamos el ultimo que fue actualizado en inserta(nodoObjetivo) ultimo nodo comun en el trie

		while (i >= 0 && !dejarDeActualizarProfundidad) {
			Link NodoAntecesorAActualizarProfundidad = nodosAAumentarProfundidad[i];

			//si el nodo ya tiene mas profundidad que el hijo actualizado +1 (hijo+el mismo) no habra que seguir
			//actualizando antecesores porque ya lo estaran ya que esta profundidad la tenia por otra ramificacion
			//distinta a la creada por la palabra que acabamos de insertar
			if (NodoAntecesorAActualizarProfundidad->altura >= profundidadNodoHijoAnterior + 1)
				dejarDeActualizarProfundidad = true;

			//si es la nueva palabra la que ha generado la mayor profundidad act el nudo progenitor
			else
				NodoAntecesorAActualizarProfundidad->altura = profundidadNodoHijoAnterior + 1;

			i--;
			profundidadNodoHijoAnterior = NodoAntecesorAActualizarProfundidad->altura;
		}
	}
	//Buscamos una palabra, devolvemos el nodo que representa el ultimo caracter de la cadena que existe en el trie.
	//funcion que explora las ramificaciones de un nodo y, dada una cadena string devuleve el nodo a partir del cual no existe una correlacion en el trie o devuelve el 
	// nodo que representa el ultimo caracter de la cadema si esa cadena ya estaba representada anteriormente en el trie
	TreeNode* search(string const& palabraBuscada, Link& nodo, vector<Link>& nodosAAumentarProfundidad) { //Ruben me dijo que con tipos pequeños como int no usara& con string si? lo hace alberto en el tad
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
				nodosAAumentarProfundidad.push_back(nodo);
				return search(palabraBuscada, NodoHijo, nodosAAumentarProfundidad);//seguimos explorando por el hijo que coincida con el caracter 

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
			//palabra[nodo->altura]=nuevoHijo->elem;
			//añadimos tamb el nuevo hijo al set de prioridad para cuando exploremos soluciones tengamos en cuenta los nodos mas prometedores anttes
			//nodo->hijosPorAlturaSet.insert(nuevoHijo);
			//añadimos al set que prioriza los nodos por su numero de palabras
			nodo->hijosPorPalabrasAlcanzablesSet.insert(nuevoHijo);

			//seguimos insertando nodos y a la vuelta de la recursiva vamos actualizando las profundidades
			nodo->altura = max(inserta(palabra, nuevoHijo) + 1, nodo->altura);// si en el nodo actual yo ya tenia un hijo mas profundo que el que acabo de expandir
			//Al volver de la recursiva añadimos a cada nodo una nueva palabra alcanzable(la que acabamos de añadir)
			nodo->numPalabrasAlcanzables += 1;
			return nodo->altura; //esto no es un poco raro? devolvemos true porque crece el arbol
		}
	}
	//exploramos los hijos dando prioridad por el numero de palabras
	void explorarPorNumeroPalabras(Link& nodo, TrieQuery& problema, SolucionLetras& solParcial) {
		
		//recorremos los hijos dando prioridad a los que mas palabras puedan formarse a partir de el 
		for (Link nodoHijo : nodo->hijosPorPalabrasAlcanzablesSet) {

			//si en el input tenemos la letra que representa el nodo y no la hemos agotado en nieveles superiores 
			//tamb hacemos una poda en la que si la longitud de la solucion parcial que llevamos + la longitud maxima alcanzable para la palabra mas larga que podriamos
			//llegar a obtener es aun asi peor que la mejor sol que ya tenemos saltamos este nodo y exploramos el siguiente
			if (problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] > 0 && nodoHijo->altura+solParcial.longitud>problema.mejorSolucion.longitud) {

				//marcadores
				solParcial.palabraSolucion[nodo->nivel] = nodoHijo->elem;
				solParcial.longitud += 1;
				problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] -= 1;

				//si tenemos una solucion mas larga y es una palabra de nuestro vocabulario
				if (solParcial.longitud > problema.mejorSolucion.longitud && nodoHijo->terminal)
					problema.mejorSolucion = solParcial;

				explorarPorNumeroPalabras(nodoHijo,problema,solParcial);

				//desmarcamos
				solParcial.palabraSolucion[nodo->nivel] = '0/'; 
				solParcial.longitud -= 1;
				
				problema.letrasDisponibleslist[MappingCharToPosition(nodoHijo->elem)] += 1;

			}

		}
	}

	//exploramos recursivamente los nodos descendentes de node y en cada nodo tratamos las posibles soluciones que generen sus hijos
	void explorar(Link& node, TrieQuery& problema, SolucionLetras& solParcial) {//struct Solucion definida en TrieQuery

		//recorremos los diferentes hijos del nodo actual
		for (Link nodoHijo : node->hijos) {

			if (nodoHijo != nullptr) {
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
					if (nodoHijo->terminal == true && nodoHijo->nivel > problema.mejorSolucion.longitud)
						problema.mejorSolucion = solParcial;//hacemos una copia de los datos para actualizar la mejor sol

					explorar(nodoHijo, problema, solParcial);

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

public:
	static void libera(Link a) {
		if (a != nullptr) {

			for (const Link& nodo : a->hijos)
				libera(nodo);

			delete a;
		}
	}

	//Verificamos si una palabra esta representada en el nodo
	//Importamte cotejar terminal puede estar representado su cadena al ser sub cadena de una ya insertada(solo-sol)
	bool existe(string const& palabra, Link const& nodo) {
		return nodo != nullptr && nodo->nivel == palabra.size() && nodo->terminal == true;
	}


};


#endif
