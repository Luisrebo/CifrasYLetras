/*@ <authors>
 *
 * Nombre, apellidos y usuario del juez (TAISXX) de los autores de la soluci�n.
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>
#include <queue>
#include <functional> 
using namespace std;

//#include "BFS.h"
/*@ <answer>

 Escribe aqu� un comentario general sobre la soluci�n, explicando c�mo
 se resuelve el problema y cu�l es el coste de la soluci�n, en funci�n
 del tama�o del problema.

 @ </answer> */


 // ================================================================
 // Escribe el c�digo completo de tu soluci�n aqu� debajo
 // ================================================================
 //@ <answer>
using num_t = short;

//En este struct vamos a guardar las soluciones
struct Solucion {
	vector<int> ordenDeUso;
	vector<char> operacionesEnOrden;
	int solMejor;
};

//En varios hay soluciones mas cortas, podriamos dejar de comprobar en cuanto consiga una solucion o adoptar una nueva estructura si contiene menos operaciones
//En cada operacion (suma resta division multiplicacion saco y meto los mismo numeros al vector y actualizo los marcadores)
//no hace falta hacerlo todo el rato de hecho es ineficiente pero es una primera version muy bruta en la que no quiero refactorizar nada

struct Operacion {
	char simbolo;  // el s�mbolo de la operaci�n
	std::function<num_t(num_t, num_t)> op;  // la operaci�n misma
	std::function<bool(num_t, num_t)> valida;  // �cu�ndo la operaci�n es v�lida?
	num_t indiceOpInversa; //el inidice de la operacion que deshace la funcion op (para dejar el resultado como estaba)
};
const array<Operacion, 4> OPERACIONES = {
	Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; },1},
	{'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; },0},
	{'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return a!=0 &&b!=0; },3 },
	{'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; },2 }
};
void DFS(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int& numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, Solucion& sol) {

	if (mejorSol == numObjetivo) {
		return;
	}

	//Hacemos todas las combinaciones de dos cifras (2 a dos) aplicandole a cada combinacion todas las operaciones posibles
	for (int i = k; i < numerosCandidatos.size() && numerosCandidatos.size() < 12; i++)
		//si i esta desmarcado y se puede operar con el
		if (!marcador[i]) {
			marcador[i] = true;
			for (int j = 0; j < numerosCandidatos.size(); j++) {

				//si j esta desmarcado y se puede operar con el y i y j son distintos
				if (!marcador[j] && i != j) {

					for (Operacion operacion : OPERACIONES) {

						//si la operacion no es valida se continua sin hacer nada
						if (operacion.valida(numerosCandidatos[i], numerosCandidatos[j])) {


							//Solo operaciones validas:

							//actualizamos la solucion parcial
							solParcial = operacion.op(numerosCandidatos[i], numerosCandidatos[j]);

							//marcamos los numeros utilizados para no repetirlos
							marcador[i] = marcador[j] = true;

							//a�adimos a numeros candidatos la nueva cifra, resultante de la op 
							numerosCandidatos.push_back(solParcial);

							//a�adimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
							marcador.push_back(false);

							//rellenamos las estructuras que nos permitir� reconstruir la solucion
							ordenDeUso.push_back(numerosCandidatos[i]);
							ordenDeUso.push_back(numerosCandidatos[j]);
							operacionesEnOrden.push_back(operacion.simbolo);

							//nos guardamos los padres de la cifra que acabamos de a�adir para reconstruir en la solucion solo las op extrictamente necesarias
							//padres.push_back(std::make_pair(i, j));

							//Actualizamos, si es necesario, la mejor solucion con respecto a la solucion parcial
							//si hay menos diferencia entre la sol parcial y el numero objetivo que entre la solucion hasta el momento se actualiza la mejor sol
							//Queremos soluciones mas cercanas aunque excedan al numero??
							if (abs(numObjetivo - solParcial) < abs(numObjetivo - mejorSol) && solParcial > 0) {
							//if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial >= 0) {
								mejorSol = solParcial;
								sol.operacionesEnOrden = operacionesEnOrden;
								sol.ordenDeUso = ordenDeUso;
								sol.solMejor = mejorSol;
								
							}//nos guardamos aqui los array para reconstruir?

							//llamada recursiva
							DFS(i, nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

						//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
						//As� como las estructuras que nos ayudan a reconstruir la solucion
						//eliminamos numerosCandidatos[j]
							ordenDeUso.pop_back();
							//eliminamos numerosCandidatos[i]
							ordenDeUso.pop_back();
							//eliminamos la operacion (+,-,*,/)
							operacionesEnOrden.pop_back();

							//eliminamos el marcador que creamos para el elemento resultado de la suma entre numerosCandidatos[i] + numerosCandidatos[j] (*no es necesario hacerlo todo el rato, lo estamos repitiendo en cada operacion*)
							marcador.pop_back();

							//eliminamos de los candidatos el resultado de operar numerosCandidatos[i] y  numerosCandidatos[j]
							numerosCandidatos.pop_back();

							//desmarcamos los numeros utilizados para poder utilizarlos
							//*deberiamos solo desmarcar la j porque la i la seguiremos utilizando*
							/*marcador[i] = */ marcador[j] = false;

							//eliminamos los padres de la sol parcial al ser esta tamb eliminada
							//padres.pop_back();

							//deshacemos la operacion con la sol parcal
						   //actualizamos la solucion parcial
							//no hace falta comprobar si se puede porque si se podia hacer se puede deshacer y, de hecho la resta no la hacemos si dos numeros 
							//solParcial = OPERACIONES[operacion.indiceOpInversa].op(i, j);	
						}
					}
				}
			}
			marcador[i] = false;

		}
}
/*En las ocasiones que el dfs encuentra una solucion mejor para ese momento guarda las operaciones que se ha realizado y si se realiza una operacion intermedia al explorar
pero esta operacion no participa en la solucion no queremos mostrarla por pantalla para ello compruebo antes de mostrar si el resultado de una operacion esta en orden de uso
si no esta es porque no participa en la sol final asique no lo mostrar� (si se volviera a utilizar para otra op no necesaria saldria pero al haber solo 6 cifras esto no podria ocurrir al no poder repetir cifras)*/
bool contained(vector<int> ordenDeUso, int cifra,int solMejor) {
	for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;

	//si la cifra no esta en orden de uso solo la mostrar� si es la solucion
	return cifra==solMejor;
}

int calcular(char operacion, int solucionparcial, int operando) {
	switch (operacion) {
	case '+':
		return solucionparcial + operando; // Sumar el operando al resultado
	case '-':
		return solucionparcial - operando; // Restar el operando al resultado
	case '*':
		return solucionparcial * operando; // Multiplicar el operando con el resultado
	case '/':
		return solucionparcial / operando; // Dividir si el operando no es cero

		//NUNCA VA A PASAR PERO PARA QUE NO CHILLE
	default:

		return -1;
	}
}


void resuelveCaso() {

	//numero a alcanzar entre 101 y 999
	int numObjetivo;

	vector<int> numerosCandidatos(6);

	cin >> numObjetivo;

	for (int i = 0; i < 6; i++)
		cin >> numerosCandidatos[i];

	vector<bool> marcador(6);
	vector<char> operacionesEnOrden;
	vector<int> ordenDeUso(0);
	//vector<pair<num_t, num_t>> padresDFS;
	Solucion sol;
	sol.solMejor = 0;
	int mejorSol = 0;

	// DFS
	DFS(0, 0, 0, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

	cout << "Numeros candidatos:( ";
	for (int i = 0; i < numerosCandidatos.size(); i++)cout << numerosCandidatos[i] << " ";
	cout << ")" << "\n";

	cout << "Numero Objetivo:" << numObjetivo << " Solucion:" << sol.solMejor;
	if (sol.solMejor != numObjetivo)cout << "\n" << "NO HAY SOLUCION EXACTA";
	cout << "\n";


	for (int i = 0, j = 0; i < sol.ordenDeUso.size() - 1; i += 2, j++) {
		//solo muestro el numero si esta contenido en las operaciones que he utilizado para desarrollar la solucion
		if(contained(sol.ordenDeUso, calcular(sol.operacionesEnOrden[j], sol.ordenDeUso[i], sol.ordenDeUso[i + 1]),sol.solMejor))
		cout << sol.ordenDeUso[i] << sol.operacionesEnOrden[j] << sol.ordenDeUso[i + 1] << "=" << calcular(sol.operacionesEnOrden[j], sol.ordenDeUso[i], sol.ordenDeUso[i + 1]) << '\n';
	}
	cout << "\n";

	//BFS bfs(sol,numObjetivo,can);
}


//@ </answer>
//  Lo que se escriba dejado de esta l�nea ya no forma parte de la soluci�n.

int main() {
	// ajustes para que cin extraiga directamente de un fichero


#ifndef DOMJUDGE
	std::ifstream in("Texto.txt");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

	int numCasos;
	std::cin >> numCasos;

	//tiempo antes de ejecutar el algoritmo
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numCasos; ++i)
		resuelveCaso();

	/*cout << sizeof(short) << endl;
	cout << sizeof(int) << endl;
	cout << sizeof(bool) << endl;*/

	//guardamos el tiempo despues de ejecutar el algoritmo
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "Tiempo en la resolucion de los " << numCasos << " casos de prueba  " << duration.count() << " milisegundos.\n";

	// para dejar todo como estaba al principio
#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
	system("PAUSE");
#endif
	return 0;
}
