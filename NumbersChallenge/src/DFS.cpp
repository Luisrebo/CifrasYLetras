#include "DFS.h"
#include "Search.h"

DFS::DFS(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos) :Search(nObjetivo, numerosCandidatos) {}

SearchResult DFS::busqueda(StatsSingleCaseCifras& stats) {

	//set de stats
	setStatsSingleCase(stats);

	resolvere(0, verticeOrigen._numCandidatos, verticeOrigen._ordenDeUso, verticeOrigen._operacionesEnOrden);
	return SearchResult(sol._problema._operacionesEnOrden, sol._problema._ordenDeUso, sol._nivel, sol._solMejor);
}

void DFS::resolvere(int nivel, std::array<num_t, CIFRAS_INICIALES>& numCandidatos, std::array <num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordenDeUso, std::array <char, CIFRAS_INICIALES>& operacionesEnOrden) {
	//si encontramos la solucion acabamos
	if (numObjetivo == sol._solMejor)
		return;

	//Actualizamos las estadísticas
	statsPtr->statsComunes.numeroDeNodosVisitados += 1;
	statsPtr->statsComunes.numeroDeNodosGenerados += 1;

	if (nivel > statsPtr->statsComunes.nivelMaximoAlcanzado)//no hace falta 
		statsPtr->statsComunes.nivelMaximoAlcanzado = nivel;

	//vamos a ir recorriendo los operandos para ir haciendo operaciones con las combinacones
	//recorremos los elementos de izq a dcha y para cada elemento operamos con los demas j=i+1
	//si no se pudiera hacer una operacion hacia un lado (i-j) probamos la operacion hacia el otro (j-1)
	for (int i = 0; i < CIFRAS_INICIALES - nivel - 1; ++i) {


		for (int j = i + 1; j < CIFRAS_INICIALES - nivel; ++j) {

			//Realizamos todas las operaciones posibles (+,-,*,/)
			for (const auto& operacion : OPERACIONES) {

				num_t posOperando1;
				num_t posOperando2;

				//Si se puede operar (suma,resta positiva,multiplicacion ,divisibles)
				//si no se puede hacer la operacion i -> j probamos a hacer la op j -> i
				//si no se puede hacer ninguna continuamos
				if (operacion.valida(numCandidatos[i], numCandidatos[j])) {
					//i op j (ejem: i-j)
					posOperando1 = i;
					posOperando2 = j;
				}
				else if (operacion.valida(numCandidatos[j], numCandidatos[i])) {
					//j op i (ejem: j-i)
					posOperando1 = j;
					posOperando2 = i;
				}
				//si no se pueden operar i con j ni j con i
				else
					continue;

				//si esta operacion es la ultima este resultado sera la solucion (numero objetivo )
				//a continuacion actualizamos los vectores antes de guardarnos la solucion(si fuera la mejor hasta el momento)
				//porque de ser asi no guardariamos tamb los array que tenemos que actualizar
				num_t solParcial = operacion.op(numCandidatos[posOperando1], numCandidatos[posOperando2]);

				//tambien me guardo la cifra que tenia en posOperando1 y 2 para restaurarla despues de la llamada recursiva
				num_t antiguaCifraI = numCandidatos[i];
				num_t antiguaCifraJ = numCandidatos[j];

				//guardo las cifras que utilice, para reconstruir la solucion:
				//en este vector de max 11 posiciones la primera cifra que guardo la meto en la pos (nvl*2) y la segunda en (nvl*2 +1)
				//en el nivel 0 guardo los operando en las pos 0 y 1 en el nvl 1 en las pos 2 y 3 en el nvl 2 en las pos 4 y 5
				//NO GUARDO EL RESULTADO DE AMBS?!
				ordenDeUso[nivel * 2] = numCandidatos[posOperando1];
				ordenDeUso[(nivel * 2) + 1] = numCandidatos[posOperando2];


				//REORDENO LAS CIFRAS (OPERANDOS)
				//Las dos cifras que acabo de usar no las vol a volver a necesitar asique voy a guardar el numero resultado de operarlas
				//en la posicion de una de ellas y el ultimo numero candidato en la posicion de la otra
				//esto me permite "rellenar" los dos huecos que dejaban las cifras que acabo de usar en este vertice y me ahorra iteraciones en los bucles
				//Ahora basta con que j empiece en i+1 al probar i op j y j op i en el mismo vertice

				//i<j es un invariante en la ejecucion del algoritmo por ello añadimos el resultado en la primera pos libre (i) 
				// y el ultimo operando de los candidatos _numCandidatos.size() - 1-vertice._nivel el segundo hueco libre (j)
				//se podrian añadir indistintamente ya que se recorrer todos los candidatos desde i=0 pero este convenio da claridad
				//la parte mas importante es tener en cuenta que el numero de candidatos solo es numerosCandidatos.size()=numerosCandidatos.size() - 1- nivel en el nivel 0
				// luego el ultimo candidato del nivel habra que encontrarlo en la pos  numerosCandidatos.size() - 1-nivel 

				//numCandidatos[i] = solParcial;
				//numCandidatos[j] = numCandidatos[numCandidatos.size() - 1 - nivel];
				numCandidatos[j] = solParcial;
				numCandidatos[i] = numCandidatos[numCandidatos.size() - 1 - nivel];
				operacionesEnOrden[nivel] = operacion.simbolo;

				//ACTUALIZO LA SOLUCION
				if (abs(numObjetivo - solParcial) < abs(numObjetivo - sol._solMejor) && solParcial > 0) { //Priorizamos soluciones que no excedan la cifra?

					//mejorSol = solParcial;
					sol._problema._ordenDeUso = ordenDeUso;
					sol._problema._operacionesEnOrden = operacionesEnOrden;
					sol._solMejor = solParcial;
					sol._problema._numCandidatos = numCandidatos;
					sol._nivel = nivel;
					statsPtr->statsComunes.nivelSolucion = nivel;
					statsPtr->statsComunes.numeroDeVecesActualizaSolucion += 1;

					//si encontramos la solucion acabamos
					if (numObjetivo == sol._solMejor)
						return;
				}

				resolvere(nivel + 1, numCandidatos, ordenDeUso, operacionesEnOrden);

				//restauro las cifras para seguir en el nivel actual y utilizarlas si fuera necesario
				//en pos1 puse el resultado que quedara sobre escrito pero hay que devolver la cifra que puse en pos2 al final
				//numCandidatos[numCandidatos.size() - 1 - nivel] = numCandidatos[j];
				numCandidatos[numCandidatos.size() - 1 - nivel] = numCandidatos[i];
				numCandidatos[i] = antiguaCifraI;
				numCandidatos[j] = antiguaCifraJ;

				//las demas estructuras se rellenan con el nivel por lo cual no es necesario restaurarlas, para ello nos guardamos
				//en la estructura sol el nivel de altura donde se obtuvo la mejor sol, para no confundirnos con celdas rellenas por otras soluciones
			}

		}
	}
	statsPtr->statsComunes.numeroDeNodosCompletamenteExplorados += 1;
}

bool DFS::contained(std::array<int, CIFRAS_INICIALES>& ordenDeUso, int cifra, int solMejor) {

	for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;
		else return false;
	return false;
}





