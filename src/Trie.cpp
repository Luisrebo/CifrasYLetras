#include "Trie.h"
#include "TrieSolver.h"
#include "TrieQuery.h"
#include "HeuristicaFactory.h"

using namespace std;

Trie& Trie::getInstance() {
	static Trie instance;
	return instance;
}

// Constructor privado: crea raíz y heurística por defecto
Trie::Trie() : raiz(new Trie::TreeNode (' ',0)), heuristica(HeuristicaFactory::crearHeuristica(TipoHeuristica::ALTURA)) {}


// Libera memoria de nodos recursivamente
void Trie::libera(Trie::Link nodo) {
	if (!nodo) return;
	for (auto* h : nodo->hijos) {
		libera(h);
	}
	delete nodo;
}
//si indexamos los vértices por la letra que ocupan en el vector de letras disponibles podemos saber si una letra esta disponible 
//para usarla mirando directamente la posicion que ocupa a traves de su ascci sabiendo que los ascii de las letras son consecutivas
//al igual que sus posiciones empezando en [0]='a'
int Trie::MappingCharToPosition(char c) {
	return c - 'a';
}

//Buscamos una palabra, devolvemos el nodo que representa el ultimo caracter de la cadena que existe en el trie.
//funcion que explora las ramificaciones de un nodo y, dada una cadena string devuleve el nodo a partir del cual no existe una correlacion en el trie o devuelve el 
// nodo que representa el ultimo caracter de la cadema si esa cadena ya estaba representada anteriormente en el trie
Trie::TreeNode* Trie::search(string const& palabraBuscada, Trie::Link nodo) { //Ruben me dijo que con tipos pequeños como int no usara& con string si? lo hace alberto en el tad
	
		if (palabraBuscada.size() == nodo->nivel)//si la altura es mayor que la longitud de la palabra es porque hemos encontrado la cadena que buscabamos ya que sino habriamos acabado antes
			return nodo;

		//cout << palabraBuscada << endl;
		//vamos a ver si el nodo actual tiene un hijo que represente la siguiente letra de la palabra, para ellos vamos a ver si tiene un
		//Link no nulo en la posicion de su array de hijos que corresponda segun el codigo ascii del caracter de la letra que buscamos 
		//representando a la a en la pos 0 y a la z en la 25 (la ñ en la pos 26 que corresponde con ('{' - 'a')
		auto NodoHijo = nodo->hijos[MappingCharToPosition(palabraBuscada[nodo->nivel])];


		//si el nodo no tiene ningun hijo con el caracter que buscamos, su posicion del array esta vacia
		if (NodoHijo == nullptr)
			return nodo;//devolvemos el nodo a partir del cual difieren las palabras del trie con la insertada osea el ultimo nodo comun

		else {

			return search(palabraBuscada, NodoHijo);//seguimos explorando por el hijo que coincida con el caracter 

		}
	}


// Inserta una palabra en el trie
void Trie::insert(const std::string& palabraOriginal,const std::string& palabraRaw) {

	builderProbabilidades.update(palabraRaw);

	Link nodoObjetivo = search(palabraRaw, raiz);

	//cuidado si queremos instertar una cadena que sea subcadena de otra existente el flag terminal no estara a true pero existira su camino
	//Cuidado podemos querer inseratr sol y que exista solo entonces el arbol de sol estara pero sin terminal=true
	//de ser asi al entrar en inserta cambiaremos el terminal del ultimo nodo a true y ya la tendremos representada

	//si la profundida crece (insertamos un camino con mas altura de la que tenia su camino maximo)
	if (!existe(palabraRaw, nodoObjetivo))
		inserta(palabraOriginal, palabraRaw,  nodoObjetivo);
}
//nodo ya es el nodo que representa el ultimo caracter de la cadena que existe en el trie (devuelto por busqueda)
	//devolvemos true si crecemos en profundidad para actualizar la profundidad de los antecesores
int Trie::inserta(string const& palabraOriginal, string const& palabraRaw,  Link nodo) {

	//verificamos si ya hemos cabado de insertar la cadena
	// o si la cadena ya estaba insertada pendiente de actualizar el flag "terminal"
	//devolvemos la profundidad que sera 0 si acabamos de insertar un nuevo elemento o palabra.size si la palabra era
	//un subconjunto de una palabra ya existente en el trie
	if (nodo->nivel == palabraRaw.size()) {
		nodo->terminal = true;
		nodo->palabraOriginal = palabraOriginal;
		return nodo->altura;
	}
	else {
		//Creamos un nuevo hijo a partir del padre que represente el primer caracter de la cadena que falte
		Link nuevoHijo = new TreeNode(palabraRaw[nodo->nivel], nodo->nivel + 1);
		//añadimos al array de hijos, en la posicion que corresponda segun su ascii al nuevo hijo
		nodo->hijos[MappingCharToPosition(palabraRaw[nodo->nivel])] = nuevoHijo;

		//seguimos insertando nodos y a la vuelta de la recursiva vamos actualizando las profundidades
		nodo->altura = max(inserta(palabraOriginal,palabraRaw, nuevoHijo) + 1, nodo->altura);// si en el nodo actual yo ya tenia un hijo mas profundo que el que acabo de expandir
		return nodo->altura; //esto no es un poco raro? devolvemos true porque crece el arbol
	}
}

// Cambia la heurística utilizada
void Trie::setHeuristica(TipoHeuristica tipo) {
	heuristica = HeuristicaFactory::crearHeuristica(tipo,builderProbabilidades.getProbabilidades());
}



// Prepara alturas y ordena hijos según heurística
void Trie::preparar() {
	// Depende la heuristica del trie preparamos en funcion de su logica interna
	heuristica->preparar(raiz);
}

// Resuelve todos los casos del flujo, línea a línea
void Trie::resuelve(std::istream& archivo)  {

	int numProblemas;
	string letrasCasoi,MODE;

	archivo >> numProblemas>>MODE;

	setHeuristica(parseTipoHeuristica(MODE));

	//despues de cargar el arbol y antes de resolver
	//necesito cargar el vector heuristica de cada nodo y ordenarlo en funcion a ella 
	//para poder explorar y encontrrar soluciones
	preparar();

	for (int i = 0; i < numProblemas; ++i) {
		archivo >> letrasCasoi;

		
		//while (archivo >> letrasCasoi)
			 auto sol=solve(letrasCasoi);
	}

	imprimirGlobalStatsResolutions();
}
//para llamarla tambien desde emscriptem
SolucionLetras Trie::solve(std::string letras)  {
	return TrieSolver::solve(raiz, letras, promediosResolucionesLetras);
}


/*void Trie::cargarDesdeArchivo(std::istream& archivo) {
	//palabra original es la palabra con tildes, dieresis etc
	//palabraRaw es la palabra parseada con todos sus componentes transformables a char 
	string palabraOriginal,palabraRaw;

	while (archivo >> palabraOriginal >> palabraRaw)  //Si la palabra tiene mas de 10 letras la descarto?
		if(palabraRaw.size()<=NUMERO_DE_LETRAS)
 			insert(palabraOriginal, palabraRaw);
	
}*/
void Trie::cargarDesdeArchivo(std::istream& archivo) {
	string palabraOriginal, palabraRaw;
	size_t leidas = 0, insertadas = 0;
	while (archivo >> palabraOriginal >> palabraRaw) {
		++leidas;
		size_t len = palabraRaw.size();
		if (len <= NUMERO_DE_LETRAS) {
			std::cout
				<< "[+] Insertando: raw=\"" << palabraRaw
				<< "\" (len=" << len << ")\n";
			insert(palabraOriginal, palabraRaw);
			++insertadas;
		}
		else {
			std::cout
				<< "[-] Descartada (> " << NUMERO_DE_LETRAS
				<< "): raw=\"" << palabraRaw
				<< "\" (len=" << len << ")\n";
		}
	}
	std::cout << "==> Leídas: " << leidas
		<< ", Insertadas: " << insertadas << "\n";
}
Trie::~Trie() {
	libera(raiz);
}
bool Trie::existe(string const& palabra, Link const& nodo) {
	return nodo != nullptr && nodo->nivel == palabra.size() && nodo->terminal == true;
}

void Trie::imprimirGlobalStatsResolutions() { promediosResolucionesLetras.imprimirResumen(); }

TipoHeuristica Trie::parseTipoHeuristica(const std::string& s) {
	if (s == "AlFABETICO" || s == "alfabetico")   return TipoHeuristica::AlFABETICO;
	if (s == "PROBABILIDADES" || s == "probabilidades") return TipoHeuristica::PROBABILIDADES;
	if (s == "ALTURA" || s == "altura")       return TipoHeuristica::ALTURA;
	else return TipoHeuristica::PALABRASALCANZABLES;

	
}