/*
#include <iostream>
#include <fstream>
#include <array>
#include <chrono>
#include <queue>
#include <functional> 
#include <vector>

using namespace std;
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
		_marcador(marcador),//no se usa en bfs
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



class BFS {
public:

	BFS(Solucion& solu, const int& nObjetivo, vector<int> numerosCandidatos):mejorSolucion(0),ordenDeUso{},operacionesEnOrden{},numObjetivo(nObjetivo),sol(solu) {
		std::copy(numerosCandidatos.begin(), numerosCandidatos.end(), numCandidatos.begin());
		marcador.fill(false);
		//marcador = { false, false, false, false, false, false };

		resolver();
	}

private:
	/*ATB*//*
	Solucion sol;
	int numObjetivo;
	queue<Problema> cola; /*cola de vertices / nodos del bfs, no exploramos el nivel i + 1 hasta tener explorado completamente el nivel i/
	num_t  mejorSolucion;//mejor solucion encontrada
	std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS> ordenDeUso ;//orden de uso de las cifras el conseguir una solucion(para la reconstruccion de la solucion)/
	std::array<char, CIFRAS_INICIALES>operacionesEnOrden;//lista de la operaciones en orden de uso (para reconstruir la solucion)/
	std::array<bool, CIFRAS_INICIALES> marcador ; //no hace falta en bfs
	
	const array<Operacion, 4> OPERACIONES = {
		Operacion{'+', [](num_t a, num_t b) { return a + b; }, [](num_t a, num_t b) { return true; }},
		{'-', [](num_t a, num_t b) { return a - b; }, [](num_t a, num_t b) { return a > b; }},
		{'*',[](num_t a, num_t b) { return a * b; }, [](num_t a, num_t b) { return true; } },
		{'/',[](num_t a, num_t b) { return a / b; }, [](num_t a, num_t b) { return a > 0 && b > 0 && a % b == 0; } }
	};
	std::array<num_t, CIFRAS_INICIALES> numCandidatos; /*Numeros candidatos pero en array estatico*/

	/*FUN*//*
	void resolver() {
	
		Problema problema(0, marcador, ordenDeUso, operacionesEnOrden, numCandidatos, 0);

		cola.push(problema);

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

	void escribirSolucionBFS() {

		cout << "Numeros candidatos:( ";
		for (int i = 0; i < CIFRAS_INICIALES; i++)cout << numCandidatos[i] << " ";
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
	
};
