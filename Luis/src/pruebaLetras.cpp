#include "pruebaLetras.h"
#include "FuncionesComunes.h"
#include "Trie.h"
#include <string>
#include <array>
#include <iostream>
#include <sstream>

using namespace std;

std::string resuelveCasoLetras(std::string letras) {

	// Obtén la única instancia del Trie mediante el singleton.
	Trie& trie = Trie::getInstance();



	// variable estática local se inicializa solo la primera vez que se ejecuta la función yuna vez asignado su valo se mantiene para todas las llamadas posteriores
	//Asi no cargamos mas de una vez el trie
	static bool diccionarioCargado = false;
	if (!diccionarioCargado) {
		ifstream archivoEntradaDiccionario("/data/diccionario_todas_sin_tildes.txt");
		if (!archivoEntradaDiccionario) {
			return "Error: no se pudo abrir el diccionario";
		}
		loadDiccionario(trie, archivoEntradaDiccionario);
		diccionarioCargado = true;
	}
	SolucionLetras solucion = trie.solve(letras);

	return formatearSolucionLetras(solucion, letras);
}
void loadDiccionario(Trie& trie, istream& archivo) {
	string palabra;

	while (archivo >> palabra)  //Si la palabra tiene mas de 10 letras la descarto?
		if (palabra.size() <= 10)  // Opcional: descartar palabras largas
			trie.insert(palabra);
}
std::string formatearSolucionLetras(SolucionLetras const& solucion, string letrasDisponibles) {
	//mejor que char* porque me ahorro malloc y calculos de memoria, destructres etc
	//va a tamaño justo. #include <sstream>
	ostringstream oss;

	if (solucion.palabraSolucion.empty())
		return "error";
	if (solucion.longitud > 0)
		oss << "SOLUCION PARA LAS LETRAS ( ";
	else
		oss << "NO HAY SOLUCION PARA LAS LETRAS: ";

	//la { por ser el consecutivo ascii de z la tratamos como ñ 
		//oss << u8"\u00F1";; inserta la ñ ignorando la codificacion
	for (char c : letrasDisponibles) {
		if (c == '{')
			oss << u8"\u00F1" << " ";
		else
			oss << c << " ";
	}


	oss << ')' << '\n';

	oss << "Longitud:" << solucion.longitud << "  ->  ";
	for (int i = 0; i < solucion.longitud; ++i) {

		//la { por ser el consecutivo ascii de z la tratamos como ñ 
		//oss << u8"\u00F1";; inserta la ñ ignorando la codificacion
		if (solucion.palabraSolucion[i] == '{')
			oss << u8"\u00F1";
		else
			oss << solucion.palabraSolucion[i];
	}

	oss << '\n' << '\n';


	return oss.str();

}
