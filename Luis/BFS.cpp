/*@ <authors>
 *
 * Nombre, apellidos y usuario del juez (TAISXX) de los autores de la solución.
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>
#include <queue>
#include <functional>  // lo necesito para std::function
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
//2bytes en vez de 4 de int
// -32,768 a 32,767.
using num_t = short;

//mejor que const?
constexpr short CIFRAS_INICIALES = 6; //cifras iniciales/disponibles/operandos
constexpr short CIFRAS_MAXIMAS_ENCADENADAS = 12; //maximo numero de cifras que van a ser utilizadas en una solucion (caso peor: caso secuencial :11)


struct Solucion {
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	num_t solMejor;
	num_t _nivel; //para reconstruir la solucion
};

struct Operacion {
	char simbolo;  // el símbolo de la operación
	std::function<num_t(num_t, num_t)> op;  // la operación misma
	std::function<bool(num_t, num_t)> valida;  // ¿cuándo la operación es válida?
};

struct Problema {
	// Constructor que inicializa todos los miembros
	Problema(num_t solParcial,
		std::array<bool, CIFRAS_INICIALES> marcador,
		std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso,
		std::array<char, CIFRAS_INICIALES> operacionesEnOrden,
		std::array<num_t, CIFRAS_INICIALES> numCandidatos,
		num_t nivel)
		: _solParcial(solParcial),
		_marcador(marcador),
		_ordenDeUso(ordenDeUso),
		_operacionesEnOrden(operacionesEnOrden),
		_numCandidatos(numCandidatos),
		_nivel(nivel)
	{}

	//la unica forma que encuentro de saber en que nivel estamos es poniendolo en el vertice
	num_t _nivel;
	num_t _solParcial;
	std::array<bool, CIFRAS_INICIALES> _marcador;
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> _ordenDeUso;
	std::array<char, CIFRAS_INICIALES> _operacionesEnOrden;
	std::array<num_t, CIFRAS_INICIALES> _numCandidatos;
};

//resolvemos con bfs

void BFS(Solucion& sol, const int& numObjetivo, vector<int> numerosCandidatos) {

	//en esta cola guardamos los nodos que vamos apilando y desapilando para recorrer nivel a nivel
	queue<Problema> cola;

	//inicialezamos el nivel y la mejor solucion obtenida
	num_t  mejorSolucion = 0;

	//vectores estaticos para mejorar el espacio. ¿En parte por la redimension automática?
// Inicialización de arrays
	std::array<bool, CIFRAS_INICIALES> marcador = { false, false, false, false, false, false }; // Inicializa con todos los valores en false

	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso = { 0 }; // Inicializa todos los elementos con el valor 0

	std::array<char, CIFRAS_INICIALES> operacionesEnOrden = {  }; // Inicializa con espacios en blanco

	// Array fijo de operaciones que podemos aplicar
	const array<Operacion, 4> OPERACIONES = {
		Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
		{'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
		{'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
		{'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; } }
	};

	//SOLUCION PROVISIONAL para pasar de array dinamico a estatico mejorando el coste en memoria
	std::array<num_t, CIFRAS_INICIALES> numCandidatos;
	if (numerosCandidatos.size() == CIFRAS_INICIALES)
		std::copy(numerosCandidatos.begin(), numerosCandidatos.end(), numCandidatos.begin());

	//problema que representa el nodo/vértice origen del bfs
	Problema problema(0, marcador, ordenDeUso, operacionesEnOrden, numCandidatos, 0);
	cola.push(problema);

	num_t idContador = 0;

	while (!cola.empty()) {

		//creo una instancia de problema donde guardare los niveles que exploro en el bfs desapilando la cola 
		//y profundizando nivel a nivel
		Problema vertice = cola.front();
		cola.pop();

		//recorremos los candidatos
		//los candidatos los vamos a ir REORGANIZANDO; cuando utilicemos dos operandos de cifras disponibles vamos a liberar dos posiciones
		// y a correr a la izquierda todos los operandos restantes, de los dos ultimos huecos uno o utilizaremos para colocar la cifra resultado de estos y el otro lo dejaremos libre
		//asi siempre empezaremos a recorrer cada nivel en 0 hasta numcandidatos.size()(-2+1)*nivel (porque utilizamos dos cifras para operar y obtenemos una de resultado) -> 0 hasta numcandidatos.size()-nivel 
		for (num_t i = 0; i < CIFRAS_INICIALES - vertice._nivel; i++) {

			//si no hemos utilizado el candidato iesimo en el vertice o en antecesores directos

				//recorremos candidatos para operar con el i
			for (num_t j = 0; j < CIFRAS_INICIALES - vertice._nivel; j++) {

				if (i != j) {//mrjor poner continue? si i==j

					//Realizamos todas las operaciones posibles (+,-,*,/)
					for (Operacion operacion : OPERACIONES) {

						//Si se puede operar (suma,resta positiva,multiplicacion ,divisibles)
						if (operacion.valida(vertice._numCandidatos[i], vertice._numCandidatos[j])) {

							//Vertice hijo a partir del padre
							Problema verticeHijo = vertice;
							verticeHijo._nivel += 1;

							//realizamos la operacion entre los dos numeros
							num_t solParcial = operacion.op(vertice._numCandidatos[i], vertice._numCandidatos[j]);

							//en pos operando1 me guardo el valor mas a la izquierda del array de los dos elementos que acabo de combinar y el otro en posOperando2
							num_t posOperando1 = min(i, j);
							num_t posOperando2 = (posOperando1 == i) ? j : i;

							//REORDENO LAS CIFRAS (OPERANDOS)
							//con este indice recorremos los operandos disponibles para dejar los huecos al final
							int indice;
							//corro todos los numeros desde el primer hueco hasta el segundo
							for (indice = posOperando1; indice < posOperando2 - 1; indice++)
								verticeHijo._numCandidatos[indice] = verticeHijo._numCandidatos[indice + 1];

							//CIFRAS_INICIALES-nivel -2 ->(-2 por los huecos que dejan los operandos utilizados)
							for (indice = posOperando2 - 1; indice < CIFRAS_INICIALES - (vertice._nivel + 2); indice++)
								verticeHijo._numCandidatos[indice] = verticeHijo._numCandidatos[indice + 2];

							//ahora añado la cifra resultante en el primer hueco libre
							verticeHijo._numCandidatos[indice] = solParcial;
							verticeHijo._operacionesEnOrden[vertice._nivel] = operacion.simbolo;

							//guardo las cifras que utilice, para reconstruir la solucion:
							//en este vector de max 11 posiciones la primera cifra que guardo la meto en la pos (nvl*2) y la segunda en (nvl*2 +1)
							//en el nivel 0 guardo los operando en las pos 0 y 1 en el nvl 1 en las pos 2 y 3 en el nvl 2 en las pos 4 y 5
							//NO GUARDO EL RESULTADO DE AMBS?!
							verticeHijo._ordenDeUso[vertice._nivel * 2] = vertice._numCandidatos[i];
							verticeHijo._ordenDeUso[(vertice._nivel * 2) + 1] = vertice._numCandidatos[j];

							//ACTUALIZO LA SOLUCION
							if (abs(numObjetivo - solParcial) < abs(numObjetivo - mejorSolucion) && solParcial > 0) { //Priorizamos soluciones que no excedan la cifra?

								mejorSolucion = solParcial;
								sol._ordenDeUso = verticeHijo._ordenDeUso;
								sol._operacionesEnOrden = verticeHijo._operacionesEnOrden;
								sol.solMejor = mejorSolucion;
								sol._nivel = vertice._nivel; //para reconstruir la solucion

								//si encontramos la solucion acabamos
								if (numObjetivo == mejorSolucion)
									return;
							}

							//apilo el vertice en la cola para seguir explorandolo cuando acabe de explorar el nivel actual
							//el nivel extra no lo apilo para ahorrar en tiempo ya que seria absurdo
							if (vertice._nivel < 5)
								cola.push(verticeHijo);
						}
					}

				}
			}
		}
	}
}

//De aqui para abajo se resuelve con dfs


//En varios hay soluciones mas cortas, podriamos dejar de comprobar en cuanto consiga una solucion o adoptar una nueva estructura si contiene menos operaciones
//En cada operacion (suma resta division multiplicacion saco y meto los mismo numeros al vector y actualizo los marcadores)
//no hace falta hacerlo todo el rato de hecho es ineficiente pero es una primera version muy bruta en la que no quiero refactorizar nada


num_t calcular(char operacion, num_t solucionParcial, num_t operando) {
	switch (operacion) {
	case '+':
		return solucionParcial + operando; // Sumar el operando al resultado
	case '-':
		return solucionParcial - operando; // Restar el operando al resultado
	case '*':
		return solucionParcial * operando; // Multiplicar el operando con el resultado
	case '/':
		return solucionParcial / operando; // Dividir si el operando no es cero

		//NUNCA VA A PASAR PERO PARA QUE NO CHILLE
	default:

		return -1;
	}
}
void escribirSolucionBFS(const Solucion& sol, const vector<int>& numerosCandidatos, const int& numObjetivo) {

	cout << "Numeros candidatos:( ";
	for (int i = 0; i < numerosCandidatos.size(); i++)cout << numerosCandidatos[i] << " ";
	cout << ")" << "\n";

	cout << "Numero Objetivo:" << numObjetivo << " Solucion:" << sol.solMejor;
	if (sol.solMejor != numObjetivo)cout << "\n" << "NO HAY SOLUCION EXACTA";
	cout << "\n";

	//para llevar la cuenta
	num_t solu;
	for (int i = 0, j = 0; i <= sol._nivel * 2; i += 2, j++) {
		cout << sol._ordenDeUso[i] << sol._operacionesEnOrden[j] << sol._ordenDeUso[i + 1] << "=" << calcular(sol._operacionesEnOrden[j], sol._ordenDeUso[i], sol._ordenDeUso[i + 1]) << '\n';
	}
	cout << "\n";
}

void resuelveCaso() {

	//numero a alcanzar entre 101 y 999
   // int numObjetivo = 872; // 7+7=14 14*8=112 112+50=162 162*6=972 972-100= 872
	int numObjetivo;

	vector<int> numerosCandidatos(6);

	cin >> numObjetivo;

	for (int i = 0; i < 6; i++)
		cin >> numerosCandidatos[i];


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

	escribirSolucionBFS(sol, numerosCandidatos, numObjetivo);
}



//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main() {
	// ajustes para que cin extraiga directamente de un fichero


#ifndef DOMJUDGE
	std::ifstream in("casos.txt");
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
