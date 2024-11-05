/*@ <authors>
 *
 * Nombre, apellidos y usuario del juez (TAISXX) de los autores de la solución.
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <queue>
using namespace std;

/*@ <answer>

 Escribe aquí un comentario general sobre la solución, explicando cómo
 se resuelve el problema y cuál es el coste de la solución, en función
 del tamaño del problema.

 @ </answer> */


 // ================================================================
 // Escribe el código completo de tu solución aquí debajo
 // ================================================================
 //@ <answer>
static int o = 0;

//En este struct vamos a guardar las soluciones
struct Solucion {
	vector<int> ordenDeUso;
	vector<string> operacionesEnOrden;
	int solMejor;


};

struct Problema {
	Problema(int i, int j, int nivel, int solParcial, int mejorSol, vector<bool> marcador, vector<int> ordenDeUso, vector<string> operacionesEnOrden, vector<int> numCandidatos) {
		_i = i;
		_j = j;
		_nivel = nivel;
		_solParcial = solParcial;
		_mejorSol = mejorSol;
		_marcador = marcador;
		_ordenDeUso = ordenDeUso;
		_operacionesEnOrden = operacionesEnOrden;
		_numCandidatos = numCandidatos;
	}
	int _i;
	int _j;
	int _nivel;
	int _solParcial;
	int _mejorSol;
	vector<bool> _marcador;
	vector<int> _ordenDeUso;
	vector<string> _operacionesEnOrden;
	vector<int> _numCandidatos;
};

//resolvemos con bfs

void BFS(Solucion& sol, const int& numObjetivo, vector<int> numerosCandidatos) {

	queue<Problema> cola;


	vector<bool> marcador(numerosCandidatos.size(), false);
	vector<int> ordenDeUso;
	vector<string> operacionesEnOrden;

	//problema que representa el nodo/vértice origen del bfs
	Problema problema(0, 0, 0, 0, 0, marcador, ordenDeUso, operacionesEnOrden, numerosCandidatos);
	cola.push(problema);



	while (!cola.empty()) {
		o += 1;
		//creo una instancia de problema donde guardare los niveles que exploro en el bfs desapilando la cola 
		//y profundizando nivel a nivel
		Problema vertice = cola.front();
		cola.pop();

		//recorremos los candidatos
		for (int i = vertice._i; i < vertice._numCandidatos.size() && vertice._numCandidatos.size() < 12; i++) {

			//si no hemos utilizado el candidato iesimo en el vertice o en antecesores directos
			if (!vertice._marcador[i]) {

				vertice._marcador[i] = true;

				//recorremos candidatos para operar con el i
				for (int j = 0; j < vertice._numCandidatos.size(); j++) {

					if (!vertice._marcador[j] && i != j) {

						//marcamos i y j a usado ya que los vamos a combinar en todas las operaciones
						vertice._marcador[j] = true;

						//esta variable va aser reciclada para representar todos los hijos de vertice
						//representara el hijo de la suma,la resta,multiplicacion y division de candidatos de los indices i,j
						Problema verticeHijo = vertice;


						//SUMA

						//vertice hijo para la suma
						// verticeHijo = vertice;
						verticeHijo._i = i;
						verticeHijo._j = j;
						verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[i]);
						verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[j]);
						verticeHijo._marcador[i] = verticeHijo._marcador[j] = true;

						verticeHijo._operacionesEnOrden.push_back("+");

						verticeHijo._solParcial = vertice._numCandidatos[i] + vertice._numCandidatos[j];

						verticeHijo._numCandidatos.push_back(verticeHijo._solParcial);


						if (numObjetivo - verticeHijo._solParcial < numObjetivo - verticeHijo._mejorSol && verticeHijo._solParcial >= 0 && numObjetivo - verticeHijo._solParcial >= 0) {
							verticeHijo._mejorSol = verticeHijo._solParcial;
							sol.operacionesEnOrden = verticeHijo._operacionesEnOrden;
							sol.ordenDeUso = verticeHijo._ordenDeUso;
							sol.solMejor = verticeHijo._mejorSol;
						}//nos guardamos aqui los array para reconstruir?

						//apilamos el vertice que hizo la suma entre los numero candidatos de los indices i,j
						cola.push(verticeHijo);


						//RESTA

						//si podemos restarle al candidato i el j..
						if (vertice._numCandidatos[i] > vertice._numCandidatos[j]) {

							//reciclamos el verticeHijo para a resta como la cola hace una copia podemos hacerlo sin afectar al vertice ya apilado
							verticeHijo = vertice;
							verticeHijo._i = i;
							verticeHijo._j = j;

							verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[i]);
							verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[j]);

							verticeHijo._operacionesEnOrden.push_back("-");

							verticeHijo._solParcial = vertice._numCandidatos[i] - vertice._numCandidatos[j];

							verticeHijo._numCandidatos.push_back(verticeHijo._solParcial);


							if (numObjetivo - verticeHijo._solParcial < numObjetivo - verticeHijo._mejorSol && verticeHijo._solParcial >= 0 && numObjetivo - verticeHijo._solParcial >= 0) {
								verticeHijo._mejorSol = verticeHijo._solParcial;
								sol.operacionesEnOrden = verticeHijo._operacionesEnOrden;
								sol.ordenDeUso = verticeHijo._ordenDeUso;
								sol.solMejor = verticeHijo._mejorSol;
							}//nos guardamos aqui los array para reconstruir?

							//apilamos el vertice que hizo la resta entre los numero candidatos de los indices i,j
							cola.push(verticeHijo);
						}

						//MULTIPLICACION

						//reciclamos el verticeHijo para a multiplicacion como la cola hace una copia podemos hacerlo sin afectar al vertice ya apilado
						verticeHijo = vertice;
						verticeHijo._i = i;
						verticeHijo._j = j;

						verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[i]);
						verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[j]);

						verticeHijo._operacionesEnOrden.push_back("*");

						verticeHijo._solParcial = vertice._numCandidatos[i] * vertice._numCandidatos[j];

						verticeHijo._numCandidatos.push_back(verticeHijo._solParcial);

						if (numObjetivo - verticeHijo._solParcial < numObjetivo - verticeHijo._mejorSol && verticeHijo._solParcial >= 0 && numObjetivo - verticeHijo._solParcial >= 0) {
							verticeHijo._mejorSol = verticeHijo._solParcial;
							sol.operacionesEnOrden = verticeHijo._operacionesEnOrden;
							sol.ordenDeUso = verticeHijo._ordenDeUso;
							sol.solMejor = verticeHijo._mejorSol;
						}//nos guardamos aqui los array para reconstruir?

						//apilamos el vertice que hizo la multiplicacion entre los numero candidatos de los indices i,j
						cola.push(verticeHijo);


						//DIVISION

						//si el candidato i es divisible entre el candidato j..
						if (vertice._numCandidatos[i] % vertice._numCandidatos[j] == 0) {

							//reciclamos el verticeHijo para la division como la cola hace una copia podemos hacerlo sin afectar al vertice ya apilado
							verticeHijo = vertice;
							verticeHijo._i = i;
							verticeHijo._j = j;

							verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[i]);
							verticeHijo._ordenDeUso.push_back(vertice._numCandidatos[j]);

							verticeHijo._operacionesEnOrden.push_back("/");

							verticeHijo._solParcial = vertice._numCandidatos[i] / vertice._numCandidatos[j];

							verticeHijo._numCandidatos.push_back(verticeHijo._solParcial);

							if (numObjetivo - verticeHijo._solParcial < numObjetivo - verticeHijo._mejorSol && verticeHijo._solParcial >= 0 && numObjetivo - verticeHijo._solParcial >= 0) {
								verticeHijo._mejorSol = verticeHijo._solParcial;
								sol.operacionesEnOrden = verticeHijo._operacionesEnOrden;
								sol.ordenDeUso = verticeHijo._ordenDeUso;
								sol.solMejor = verticeHijo._mejorSol;
							}//nos guardamos aqui los array para reconstruir?

							//apilamos el vertice que hizo la division entre los numero candidatos de los indices i,j
							cola.push(verticeHijo);
						}



					}
					vertice._marcador[j] = false;
				}

			}
			vertice._marcador[i] = false;
		}

	}




}



//De aqui para abajo se resuelve con dfs


//En varios hay soluciones mas cortas, podriamos dejar de comprobar en cuanto consiga una solucion o adoptar una nueva estructura si contiene menos operaciones
//En cada operacion (suma resta division multiplicacion saco y meto los mismo numeros al vector y actualizo los marcadores)
//no hace falta hacerlo todo el rato de hecho es ineficiente pero es una primera version muy bruta en la que no quiero refactorizar nada

void recursivoBruto(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int& numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<string>& operacionesEnOrden, Solucion& sol) {
	o += 1;
	if (solParcial == numObjetivo) {
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


					//SUMA

					//actualizamos la solucion parcial
					solParcial = numerosCandidatos[i] + numerosCandidatos[j];

					//marcamos los numeros utilizados para no repetirlos
					//*Con marcar una vez  la i y j valdria no hace falta hacerlo antes y despues de cada operacion *
					marcador[i] = marcador[j] = true;

					//añadimos a numeros candidatos la nueva cifra, resultante de la suma anterior
					numerosCandidatos.push_back(numerosCandidatos[i] + numerosCandidatos[j]);

					//añadimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
					marcador.push_back(false);

					//rellenamos las estructuras que nos permitirá reconstruir la solucion
					ordenDeUso.push_back(numerosCandidatos[i]);
					ordenDeUso.push_back(numerosCandidatos[j]);
					operacionesEnOrden.push_back("+");

					//Actualizamos, si es necesario, la mejor solucion con respecto a la solucion parcial
					//si hay menos diferencia entre la sol parcial y el numero objetivo que entre la solucion hasta el momento se actualiza la mejor sol
					//Queremos soluciones mas cercanas aunque excedan al numero??
					if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial >= 0) {
						mejorSol = solParcial;
						sol.operacionesEnOrden = operacionesEnOrden;
						sol.ordenDeUso = ordenDeUso;
						sol.solMejor = mejorSol;
					}//nos guardamos aqui los array para reconstruir?

					//llamada recursiva
					recursivoBruto(i, nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

					//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
					//Así como las estructuras que nos ayudan a reconstruir la solucion
					//eliminamos numerosCandidatos[j]
					ordenDeUso.pop_back();
					//eliminamos numerosCandidatos[i]
					ordenDeUso.pop_back();
					//eliminamos la operacion +
					operacionesEnOrden.pop_back();

					//eliminamos el marcador que creamos para el elemento resultado de la suma entre numerosCandidatos[i] + numerosCandidatos[j] (*no es necesario hacerlo todo el rato, lo estamos repitiendo en cada operacion*)
					marcador.pop_back();

					//eliminamos de los candidatos la suma numerosCandidatos[i] + numerosCandidatos[j]
					numerosCandidatos.pop_back();

					//desmarcamos los numeros utilizados para poder utilizarlos
					//*deberiamos solo desmarcar la j porque la i la seguiremos utilizando*
					/*marcador[i] = */ marcador[j] = false;

					//deshacemos la operacion con la sol parcal
					//actualizamos la solucion parcial
					solParcial -= numerosCandidatos[i] + numerosCandidatos[j];


					//RESTA

					//no trabajamos con numeros negativos
					if (numerosCandidatos[i] > numerosCandidatos[j]) {

						//actualizamos la solucion parcial
						solParcial = numerosCandidatos[i] - numerosCandidatos[j];

						//marcamos los numeros utilizados para no repetirlos
						marcador[i] = marcador[j] = true;

						//añadimos a numeros candidatos la nueva cifra, resultante de la resta anterior
						numerosCandidatos.push_back(numerosCandidatos[i] - numerosCandidatos[j]);

						//añadimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
						marcador.push_back(false);

						//rellenamos las estructuras que nos permitirá reconstruir la solucion
						ordenDeUso.push_back(numerosCandidatos[i]);
						ordenDeUso.push_back(numerosCandidatos[j]);
						operacionesEnOrden.push_back("-");

						//Actualizamos, si es necesario, la mejor solucion con respecto a la solucion parcial
						//si hay menos diferencia entre la sol parcial y el numero objetivo que entre la solucion hasta el momento se actualiza la mejor sol
						//Queremos soluciones mas cercanas aunque excedan al numero??
						if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial>0) {
							mejorSol = solParcial;
							sol.operacionesEnOrden = operacionesEnOrden;
							sol.ordenDeUso = ordenDeUso;
							sol.solMejor = mejorSol;
						}//nos guardamos aqui los array para reconstruir?

						//llamada recursiva
						recursivoBruto(i, nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

						//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
						//Así como las estructuras que nos ayudan a reconstruir la solucion
						//eliminamos numerosCandidatos[j]
						ordenDeUso.pop_back();
						//eliminamos numerosCandidatos[i]
						ordenDeUso.pop_back();
						//eliminamos la operacion -
						operacionesEnOrden.pop_back();

						//eliminamos el marcador que creamos para el elemento resultado de la resta entre numerosCandidatos[i] - numerosCandidatos[j] (*no es necesario hacerlo todo el rato, lo estamos repitiendo en cada operacion*)
						marcador.pop_back();

						//eliminamos de los candidatos la resta numerosCandidatos[i] - numerosCandidatos[j]
						numerosCandidatos.pop_back();

						//desmarcamos los numeros utilizados para poder utilizarlos
						//*deberiamos solo desmarcar la j porque la i la seguiremos utilizando*
						/*marcador[i] = */  marcador[j] = false;

						//deshacemos la operacion con la sol parcal
						//actualizamos la solucion parcial
						solParcial += numerosCandidatos[i] + numerosCandidatos[j];
					}


					//MULTIPLICACION

					//actualizamos la solucion parcial
					solParcial = numerosCandidatos[i] * numerosCandidatos[j];

					//marcamos los numeros utilizados para no repetirlos
					//*Con marcar una vez  la i y j valdria no hace falta hacerlo antes y despues de cada operacion *
					/*marcador[i] = */  marcador[j] = true;

					//añadimos a numeros candidatos la nueva cifra, resultante de la multiplicacion anterior
					numerosCandidatos.push_back(numerosCandidatos[i] * numerosCandidatos[j]);

					//añadimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
					marcador.push_back(false);

					//rellenamos las estructuras que nos permitirá reconstruir la solucion
					ordenDeUso.push_back(numerosCandidatos[i]);
					ordenDeUso.push_back(numerosCandidatos[j]);
					operacionesEnOrden.push_back("*");

					//Actualizamos, si es necesario, la mejor solucion con respecto a la solucion parcial
					//si hay menos diferencia entre la sol parcial y el numero objetivo que entre la solucion hasta el momento se actualiza la mejor sol
					//Queremos soluciones mas cercanas aunque excedan al numero??
					if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial>0) {
						mejorSol = solParcial;
						sol.operacionesEnOrden = operacionesEnOrden;
						sol.ordenDeUso = ordenDeUso;
						sol.solMejor = mejorSol;
					}//nos guardamos aqui los array para reconstruir?

					//llamada recursiva
					recursivoBruto(i, nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

					//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
					//Así como las estructuras que nos ayudan a reconstruir la solucion
					//eliminamos numerosCandidatos[j]
					ordenDeUso.pop_back();
					//eliminamos numerosCandidatos[i]
					ordenDeUso.pop_back();
					//eliminamos la operacion *
					operacionesEnOrden.pop_back();

					//eliminamos el marcador que creamos para el elemento resultado de la resta entre numerosCandidatos[i] * numerosCandidatos[j] (*no es necesario hacerlo todo el rato, lo estamos repitiendo en cada operacion*)
					marcador.pop_back();

					//eliminamos de los candidatos la resta numerosCandidatos[i] * numerosCandidatos[j]
					numerosCandidatos.pop_back();

					//desmarcamos los numeros utilizados para poder utilizarlos
					//*deberiamos solo desmarcar la j porque la i la seguiremos utilizando*
					/*marcador[i] = */  marcador[j] = false;

					//deshacemos la operacion con la sol parcal
					//actualizamos la solucion parcial
					//valdria con ponerla a 0
					solParcial /= numerosCandidatos[i] * numerosCandidatos[j];

					//DIVISION

					//si es divisible
					if (numerosCandidatos[i] % numerosCandidatos[j] == 0) {

						//actualizamos la solucion parcial
						solParcial = numerosCandidatos[i] / numerosCandidatos[j];

						//marcamos los numeros utilizados para no repetirlos
						//*Con marcar una vez  la i y j valdria no hace falta hacerlo antes y despues de cada operacion *
						marcador[i] = marcador[j] = true;

						//añadimos a numeros candidatos la nueva cifra, resultante de la division anterior
						numerosCandidatos.push_back(numerosCandidatos[i] / numerosCandidatos[j]);

						//añadimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
						marcador.push_back(false);

						//rellenamos las estructuras que nos permitirá reconstruir la solucion
						ordenDeUso.push_back(numerosCandidatos[i]);
						ordenDeUso.push_back(numerosCandidatos[j]);
						operacionesEnOrden.push_back("/");

						//Actualizamos, si es necesario, la mejor solucion con respecto a la solucion parcial
						//si hay menos diferencia entre la sol parcial y el numero objetivo que entre la solucion hasta el momento se actualiza la mejor sol
						//Queremos soluciones mas cercanas aunque excedan al numero??
						if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial >= 0) {
							mejorSol = solParcial;
							sol.operacionesEnOrden = operacionesEnOrden;
							sol.ordenDeUso = ordenDeUso;
							sol.solMejor = mejorSol;
						}//nos guardamos aqui los array para reconstruir?

					//llamada recursiva
						recursivoBruto(i, nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

						//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
						//Así como las estructuras que nos ayudan a reconstruir la solucion
						//eliminamos numerosCandidatos[j]
						ordenDeUso.pop_back();
						//eliminamos numerosCandidatos[i]
						ordenDeUso.pop_back();
						//eliminamos la operacion /
						operacionesEnOrden.pop_back();

						//eliminamos el marcador que creamos para el elemento resultado de la resta entre numerosCandidatos[i] / numerosCandidatos[j] (*no es necesario hacerlo todo el rato, lo estamos repitiendo en cada operacion*)
						marcador.pop_back();

						//eliminamos de los candidatos la resta numerosCandidatos[i] - numerosCandidatos[j]
						numerosCandidatos.pop_back();

						//desmarcamos los numeros utilizados para poder utilizarlos
						//*deberiamos solo desmarcar la j porque la i la seguiremos utilizando*
						/*marcador[i] = */  marcador[j] = false;

						//deshacemos la operacion con la sol parcal
						//actualizamos la solucion parcial
						//valdria con igualarlo a 0
						solParcial *= numerosCandidatos[i] / numerosCandidatos[j];


					}


					//NO OPERAR
					//no utilizamos la cifra numerosCandidatos[i] *APORTA ALGO??*


				}


			}
			marcador[i] = false;

		}

}
int calcular(string operacion, int solucionparcial, int operando) {
	if (operacion == "+") {
		return solucionparcial += operando; // Sumar el operando al resultado
	}
	else if (operacion == "-") {
		return solucionparcial -= operando; // Resta (puedes definir cómo manejarlo)
	}
	else if (operacion == "*") {
		return solucionparcial *= operando; // Multiplicación (inicializa el resultado adecuadamente)
	}
	else if (operacion == "/") {

		return solucionparcial /= operando;
	}
}

void resuelveCaso() {

	//numero a alcanzar entre 101 y 999
   // int numObjetivo = 872; // 7+7=14 14*8=112 112+50=162 162*6=972 972-100= 872
	int numObjetivo;

	vector<int> numerosCandidatos(6);

	cin >> numObjetivo;

	for (int i = 0; i < 6; i++)
		cin >> numerosCandidatos[i];
	/* numerosCandidatos.push_back(8);
	 numerosCandidatos.push_back(7);
	 numerosCandidatos.push_back(6);
	 numerosCandidatos.push_back(50);
	 numerosCandidatos.push_back(100);
	 numerosCandidatos.push_back(7);*/

	 //NO HAY EXACTO
	/* int numObjetivo = 902;
	 numerosCandidatos.push_back(25);
	 numerosCandidatos.push_back(7);
	 numerosCandidatos.push_back(10);
	 numerosCandidatos.push_back(1);
	 numerosCandidatos.push_back(9);
	 numerosCandidatos.push_back(1);*/

	 //lo hace bien pero existe uno con menos operaciones 50+10=60,60-4,56*6,336+5=341
  /*  int numObjetivo = 341;
	numerosCandidatos.push_back(6);
	numerosCandidatos.push_back(50);
	numerosCandidatos.push_back(3);
	numerosCandidatos.push_back(10);
	numerosCandidatos.push_back(5);
	numerosCandidatos.push_back(4);*/

	vector<bool> marcador(6);
	vector<string> operacionesEnOrden;
	vector<int> ordenDeUso(0);
	Solucion sol;
	sol.solMejor = 0;
	int mejorSol = 0;

	// DFS
	//recursivoBruto(0, 0, 0, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

	//BFS Solucion& sol, const int& numObjetivo, vector<int>& numerosCandidatos
	BFS(sol, numObjetivo, numerosCandidatos);
	cout << "o=" << o << "\n";
	cout << "Numeros candidatos:( ";
	for (int i = 0; i < numerosCandidatos.size(); i++)cout << numerosCandidatos[i] << " ";
	cout << ")" << "\n";

	cout << "Numero Objetivo:" << numObjetivo << " Solucion:" << sol.solMejor;
	if (sol.solMejor != numObjetivo)cout << "\n" << "NO HAY SOLUCION EXACTA";
	cout << "\n";

	//para llevar la cuenta
	int solu;
	for (int i = 0, j = 0; i < sol.ordenDeUso.size() - 1; i += 2, j++) {
		cout << sol.ordenDeUso[i] << sol.operacionesEnOrden[j] << sol.ordenDeUso[i + 1] << "=" << calcular(sol.operacionesEnOrden[j], sol.ordenDeUso[i], sol.ordenDeUso[i + 1]) << '\n';
	}
	cout << "\n";
}

//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

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
