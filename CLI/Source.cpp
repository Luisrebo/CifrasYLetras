#include <iostream>
#include <fstream>
#include <sstream>//para el formateo de solucion hacia html
#include <vector>
#include <array>
#include <chrono>
#include <queue>
#include <algorithm>
#include <string>
#include <cctype>
#include <functional>  // lo necesito para std::function

#include "commonFunctions.h"
#include "SearchNumbers.h"
#include "Trie.h"
/*Segunda parte letras*/ //compilar ctl+shift+b
using namespace std;
//
void resuelveCasoCifras( istream& archivo ) {
	SearchNumbers busquedaCifras;
	busquedaCifras.resuelve(archivo);
}

/*Segunda Parte Letras*/
void loadDiccionario(Trie& trie, istream& archivo) {
	trie.cargarDesdeArchivo(archivo);
}
void resuelveCasoLetras(Trie& trie, istream& archivo) {
	trie.resuelve(archivo);
}


//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main(int argc, char* argv[]) {
	// Verificp si se paso un archivo como argumento
	if (argc < 4) {
		std::cerr << "Uso: " << argv[0] << " <archivo_casos.txt> <archivo_diccionario.txt> <archivo_casos_letras.txt>" << std::endl;
		return 1;
	}


	string nombreArchivo = argv[1];

	// Abrimos el archivo
	ifstream archivoEntrada(nombreArchivo);
	if (!archivoEntrada) {
		cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
		return 1;
	}

	//tiempo antes de ejecutar el algoritmo
	auto start = std::chrono::high_resolution_clock::now();
	resuelveCasoCifras(archivoEntrada);

	/*Segunda parte letras*/
	Trie& trie = Trie::getInstance();

	//cargamos los datos del diccionario
	string nombreArchivoDiccionario = argv[2];

	cout << "Leyendo el diccionario del archivo " << nombreArchivoDiccionario << endl;

	// Abrimos el archivo
	ifstream archivoEntradaDiccionario(nombreArchivoDiccionario);
	if (!archivoEntradaDiccionario) {
		cerr << "Error: No se pudo abrir el archivo " << nombreArchivoDiccionario << endl;
		return 1;
	}

	loadDiccionario(trie, archivoEntradaDiccionario);

	//cargamos los datos del diccionario
	string nombreArchivoCasosLetras = argv[3];
	// Abrimos el archivo
	ifstream archivoCasosLetras(nombreArchivoCasosLetras);
	if (!archivoCasosLetras) {
		cerr << "Error: No se pudo abrir el archivo " << nombreArchivoCasosLetras << endl;
		return 1;
	}
	// Redirigir la entrada estándar desde el archivo
	auto cinbuf = cin.rdbuf(archivoCasosLetras.rdbuf());

	resuelveCasoLetras(trie, archivoCasosLetras);

	//guardamos el tiempo despues de ejecutar el algoritmo
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "tiempo resolucion total del programa: " << duration.count()<< '\n';
	

	
	return 0;
}