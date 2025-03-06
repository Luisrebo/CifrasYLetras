#include "BFS.h"
#include "Search.h"



BFS::BFS(const num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos)
	: Search(nObjetivo, numerosCandidatos),
	mejorSolucion(0),
	verticeOrigen(0,
		std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS>{},
		std::array<char, CIFRAS_INICIALES>{},
		numerosCandidatos),
	sol(verticeOrigen, 0, 0) {
}

void BFS::busqueda() {
	// Por ejemplo, simplemente llamamos a nuestra lógica
	resolver();
	mostrarBFS();
}
void BFS::resolver() {

	num_t verticesDelNivelAnteriorPorExplorar,verticesNivelSiguiente,nivelActual;
	verticesNivelSiguiente = 0;
	nivelActual = 0;
	
	verticesDelNivelAnteriorPorExplorar = 1;
	cola.push(verticeOrigen);

	while (!cola.empty()) {

		if (verticesDelNivelAnteriorPorExplorar == 0) {
			nivelActual += 1;
			verticesDelNivelAnteriorPorExplorar = verticesNivelSiguiente;
			verticesNivelSiguiente = 0;
		}
			

		//creo una instancia de problema donde guardare los niveles que exploro en el bfs desapilando la cola
		//y profundizando nivel a nivel
		Problema vertice = cola.front();
		cola.pop();
		verticesDelNivelAnteriorPorExplorar--;

		//recorremos los candidatos
		//los candidatos los vamos a ir REORGANIZANDO; cuando utilicemos dos operandos de cifras disponibles vamos a liberar dos posiciones
		// y a correr a la izquierda todos los operandos restantes, de los dos ultimos huecos uno o utilizaremos para colocar la cifra resultado de estos y el otro lo dejaremos libre
		//asi siempre empezaremos a recorrer cada nivel en 0 hasta numcandidatos.size()(-2+1)*nivel (porque utilizamos dos cifras para operar y obtenemos una de resultado) -> 0 hasta numcandidatos.size()-nivel
		for (num_t i = 0; i < CIFRAS_INICIALES - nivelActual; i++) {

			//si no hemos utilizado el candidato iesimo en el vertice o en antecesores directos

				//recorremos candidatos para operar con el i
			for (num_t j = i + 1; j < CIFRAS_INICIALES - nivelActual; j++) {

				//Realizamos todas las operaciones posibles (+,-,*,/)
				for (Operacion operacion : OPERACIONES) {

					num_t posOperando1;
					num_t posOperando2;

					//si no se pueden operar i con j ni j con i
					if (!operacion.valida(vertice._numCandidatos[i], vertice._numCandidatos[j]) && !operacion.valida(vertice._numCandidatos[j], vertice._numCandidatos[i]))
						continue;
					//Si se puede operar (suma,resta positiva,multiplicacion ,divisibles)
					//si no se puede hacer la operacion i -> j probamos a hacer la op j -> i
					//si no se puede hacer ninguna continuamos
					else if (operacion.valida(vertice._numCandidatos[i], vertice._numCandidatos[j])) {
						//i op j (ejem: i-j)
						posOperando1 = i;
						posOperando2 = j;
					}

					else if (operacion.valida(vertice._numCandidatos[j], vertice._numCandidatos[i])) {
						//j op i (ejem: j-i)
						posOperando1 = j;
						posOperando2 = i;
					}

					//Vertice hijo a partir del padre
					Problema verticeHijo = vertice;
					

					//realizamos la operacion entre los dos numeros
					num_t solParcial = operacion.op(vertice._numCandidatos[posOperando1], vertice._numCandidatos[posOperando2]);

					//REORDENO LAS CIFRAS (OPERANDOS)
					//Las dos cifras que acabo de usar no las vol a volver a necesitar asique voy a guardar el numero resultado de operarlas
					//en la posicion de una de ellas y el ultimo numero candidato en la posicion de la otra
					//esto me permite "rellenar" los dos huecos que dejaban las cifras que acabo de usar en este vertice y me ahorra iteraciones en los bucles
					//Ahora basta con que j empiece en i+1 al probar i op j y j op i en el mismo vertice

					//i<j es un invariante en la ejecucion del algoritmo por ello añadimos el resultado en la primera pos libre (i) 
					// y el ultimo operando de los candidatos _numCandidatos.size() - 1-vertice._nivel el segundo hueco libre (j)
					//se podrian añadir indistintamente ya que se recorrer todos los candidatos desde i=0 pero este convenio da claridad
					//la parte mas importante es tener en cuenta que el numero de candidatos solo es _numCandidatos.size()=_numCandidatos.size() - 1-vertice._nivel en el nivel 0
					// luego el ultimo candidato del nivel habra que encontrarlo en la pos  _numCandidatos.size() - 1-vertice._nivel 
					verticeHijo._numCandidatos[i] = solParcial;
					verticeHijo._numCandidatos[j] = verticeHijo._numCandidatos[vertice._numCandidatos.size() - 1 - nivelActual];
					verticeHijo._operacionesEnOrden[nivelActual] = operacion.simbolo;

					//guardo las cifras que utilice, para reconstruir la solucion:
					//en este vector de max 11 posiciones la primera cifra que guardo la meto en la pos (nvl*2) y la segunda en (nvl*2 +1)
					//en el nivel 0 guardo los operando en las pos 0 y 1 en el nvl 1 en las pos 2 y 3 en el nvl 2 en las pos 4 y 5
					//NO GUARDO EL RESULTADO DE AMBS?!
					verticeHijo._ordenDeUso[nivelActual * 2] = vertice._numCandidatos[posOperando1];
					verticeHijo._ordenDeUso[(nivelActual * 2) + 1] = vertice._numCandidatos[posOperando2];

					//ACTUALIZO LA SOLUCION
					if (abs(numObjetivo - solParcial) < abs(numObjetivo - mejorSolucion) && solParcial > 0) { //Priorizamos soluciones que no excedan la cifra?

						mejorSolucion = solParcial;
						sol._problema._ordenDeUso = verticeHijo._ordenDeUso;
						sol._problema._operacionesEnOrden = verticeHijo._operacionesEnOrden;
						sol._solMejor = mejorSolucion;
						sol._nivel = nivelActual; //para reconstruir la solucion

						//si encontramos la solucion acabamos
						if (numObjetivo == mejorSolucion)
							return;
					}

					//apilo el vertice en la cola para seguir explorandolo cuando acabe de explorar el nivel actual
					//el nivel extra no lo apilo para ahorrar en tiempo ya que seria absurdo
					if (nivelActual < CIFRAS_INICIALES - 1) {
						cola.push(verticeHijo);
						verticesNivelSiguiente ++;
					}
				}
			}
		}
	}

}

void BFS::mostrarBFS() {
	cout << "Numeros candidatos:( ";
	for (int i = 0; i < CIFRAS_INICIALES; i++)cout << verticeOrigen._numCandidatos[i] << " ";
	cout << ")" << "\n";

	cout << "Numero Objetivo:" << numObjetivo << " Solucion:" << sol._solMejor;
	if (sol._solMejor != numObjetivo)cout << "\n" << "NO HAY SOLUCION EXACTA";
	cout << "\n";

	//para llevar la cuenta
	for (int i = 0, j = 0; i <= sol._nivel * 2; i += 2, j++) {
		cout << sol._problema._ordenDeUso[i] << sol._problema._operacionesEnOrden[j] << sol._problema._ordenDeUso[i + 1] << "=" << calcular(sol._problema._operacionesEnOrden[j], sol._problema._ordenDeUso[i], sol._problema._ordenDeUso[i + 1]) << '\n';
	}
	cout << "\n";
}