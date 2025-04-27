#include "pruebaLetras.h"
#include "FuncionesComunes.h"
#include "Trie.h"
#include <string>
#include <array>
#include <iostream>
#include <sstream>

using namespace std;

SolucionLetras resuelveCasoLetras(const std::string letras) {
	static bool diccionarioCargado = false;
	Trie& trie = Trie::getInstance();

	if (!diccionarioCargado) {
		std::ifstream f("data/diccionario_todas_sin_tildes.txt");
		if (!f.is_open()) {
			std::cerr << "ERROR: no pude abrir data/diccionario_todas_sin_tildes.txt\n";
			// Aquí podrías lanzar una excepción, abortar o retornar un valor por defecto
			return {};
		}
		trie.cargarDesdeArchivo(f);
		diccionarioCargado = true;
		//std::cout << "Diccionario cargado correctamente.\n";
	}

	trie.preparar();
	return trie.solve(letras);
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
