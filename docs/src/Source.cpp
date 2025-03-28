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

#include "FuncionesComunes.h"
#include "BFS.h"
#include "DFS.h"
#include "Factory.h"

/*Segunda parte letras*/ //compilar ctl+shift+b
#include "Trie.h"

using namespace std;
//
void resuelveCaso(string mode, istream& archivo ) {

	//numero al que queremos llegar o aproximarnos lo maximo posible
	int numObjetivo;

	//cifras con las que hemos de operar
	array<num_t, CIFRAS_INICIALES> numerosCandidatos;
	
	archivo >> numObjetivo;

	for (int i = 0; i < CIFRAS_INICIALES; ++i)
		archivo >> numerosCandidatos[i];

	auto search = Factory::create(mode, numObjetivo, numerosCandidatos);

	SearchResult result= search->busqueda();

	

}

/*Segunda Parte Letras*/
void loadDiccionario(Trie& trie, istream& archivo) {
	string palabra;
	
	while (archivo >> palabra)  //Si la palabra tiene mas de 10 letras la descarto?
		trie.insert(palabra);
	

	
}
void resuelveCasoLetras(Trie& trie, istream& archivo) {
	cout << "Resolviendo con poda" << '\n';
	string letrasCasoi;
	int numProblemas;
	archivo >> numProblemas;

	for (int i = 0; i < numProblemas; ++i) {
		archivo >> letrasCasoi;

		trie.solve(letrasCasoi);
	}
}


//@ </answer>
//  Lo que se escriba dejado de esta l�nea ya no forma parte de la soluci�n.

int main(int argc, char* argv[]) {
	// ajustes para que cin extraiga directamente de un fichero

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

	// Redirigir la entrada est�ndar desde el archivo
	//auto cinbuf = cin.rdbuf(archivoEntrada.rdbuf());
	

	int numCasos;
	string mode;

	archivoEntrada >> numCasos;
	archivoEntrada >> mode;

	//tiempo antes de ejecutar el algoritmo
	auto start = std::chrono::high_resolution_clock::now();

	cout << "Solucion para el archivo: " << nombreArchivo << " en modo " << mode << '\n';

	for (int i = 0; i < numCasos; ++i)
		resuelveCaso(mode, archivoEntrada);

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
	// Redirigir la entrada est�ndar desde el archivo
	//auto cinbuf = cin.rdbuf(archivoEntradaDiccionario.rdbuf());

	loadDiccionario(trie, archivoEntradaDiccionario);

	//cargamos los datos del diccionario
	string nombreArchivoCasosLetras = argv[3];
	// Abrimos el archivo
	ifstream archivoCasosLetras(nombreArchivoCasosLetras);
	if (!archivoCasosLetras) {
		cerr << "Error: No se pudo abrir el archivo " << nombreArchivoCasosLetras << endl;
		return 1;
	}
	// Redirigir la entrada est�ndar desde el archivo
	auto cinbuf = cin.rdbuf(archivoCasosLetras.rdbuf());

	resuelveCasoLetras(trie, archivoCasosLetras);

	//guardamos el tiempo despues de ejecutar el algoritmo
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "Tiempo en la resolucion de los " << numCasos << " casos de prueba  " << duration.count() << " milisegundos.\n";

	
	return 0;
}
/* LA UTILIZO EN LA INTERFAZ CON EMSCRIPTEN
string formatearSolucion(SearchResult result){
	//mejor que char* porque me ahorro malloc y calculos de memoria, destructres etc
	//va a tama�o justo. #include <sstream>
	ostringstream oss;

	//basta con un indice ya que siempre es el doble pero para mayor claridad
	for (int indiceCifras = 0, indiceOperaciones = 0; indiceCifras < result._ordendeUso.size() &&
		indiceOperaciones < result._operacionesEnOrden.size(); indiceCifras += 2, ++indiceOperaciones) {

		oss << result._ordendeUso[indiceCifras] << result._operacionesEnOrden[indiceOperaciones] << result._ordendeUso[indiceCifras + 1]
			<< "=" << calcular(result._operacionesEnOrden[indiceOperaciones], result._ordendeUso[indiceCifras], result._ordendeUso[indiceCifras + 1])
			<< '\n';
	}
	return oss.str();

}*/