#include "DFS.h"
#include "Search.h"
#include <iostream>



DFS::DFS(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos) :Search(nObjetivo, numerosCandidatos), problemaBase(0,{},{},numerosCandidatos),
	sol(problemaBase, 0, 0) {

}

void DFS::busqueda() {
	resolvere(0, problemaBase._numCandidatos, problemaBase._ordenDeUso, problemaBase._operacionesEnOrden);
	mostrarDFS();
}

void DFS::resolvere(int nivel, array<num_t, CIFRAS_INICIALES>& numCandidatos, array <num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordenDeUso, array <char, CIFRAS_INICIALES>& operacionesEnOrden) {

	//si llegamos a la mejor solucion nos detenemos

		//vamos a ir recorriendo los operandos para ir haciendo operaciones con las combinacones
		//recorremos los elementos de izq a dcha y para cada elemento operamos con los demas j=i+1
		//si no se pudiera hacer una operacion hacia un lado (i-j) probamos la operacion hacia el otro (j-1)
	for (int i = 0; i < CIFRAS_INICIALES - nivel-1; ++i) {


		for (int j = i + 1; j < CIFRAS_INICIALES - nivel; ++j) {

			//Realizamos todas las operaciones posibles (+,-,*,/)
			for (Operacion operacion : OPERACIONES) {

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

				/*Depuraion
				for (int d = 0; d < CIFRAS_INICIALES; d++)
					cout << " " << numCandidatos[d];
				cout << endl;

				for (int d = 0; d <= (nivel * 2) + 1; d++)
					cout << " " << ordenDeUso[d];

				cout << "   ->nivel " << nivel << " solparcial " << solParcial;

				if (solParcial < 0)
					cout << " " << numCandidatos[posOperando1] << " " << operacion.simbolo << " " << numCandidatos[posOperando1];

				cout << endl << endl;;
				Depuracion fin*/

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

				numCandidatos[i] = solParcial;
				numCandidatos[j] = numCandidatos[numCandidatos.size() - 1 - nivel];
				operacionesEnOrden[nivel] = operacion.simbolo;

				//ACTUALIZO LA SOLUCION
				if (abs(numObjetivo - solParcial) < abs(numObjetivo - sol._solMejor) && solParcial > 0) { //Priorizamos soluciones que no excedan la cifra?

					//mejorSol = solParcial;
					sol._problema._ordenDeUso = ordenDeUso;
					sol._problema._operacionesEnOrden = operacionesEnOrden;
					sol._solMejor = solParcial;
					sol._problema._numCandidatos = numCandidatos;
					sol._nivel = nivel;

					//si encontramos la solucion acabamos
					if (numObjetivo == sol._solMejor)
						return;
				}

				resolvere(nivel+1, numCandidatos, ordenDeUso, operacionesEnOrden);

				//restauro las cifras para seguir en el nivel actual y utilizarlas si fuera necesario
				//en pos1 puse el resultado que quedara sobre escrito pero hay que devolver la cifra que puse en pos2 al final
				numCandidatos[numCandidatos.size() - 1 - nivel] = numCandidatos[j];
				numCandidatos[i] = antiguaCifraI;
				numCandidatos[j] = antiguaCifraJ;

				//las demas estructuras se rellenan con el nivel por lo cual no es necesario restaurarlas, para ello nos guardamos
				//en la estructura sol el nivel de altura donde se obtuvo la mejor sol, para no confundirnos con celdas rellenas por otras soluciones
			}
		}
	}
}

//funcion recursiva 
//void DFS::resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol) {
/* void DFS::resolver(int k, int nivel, int solParcial, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden) {
	if (mejorSol == numObjetivo) {
		return;
	}

	//Hacemos todas las combinaciones de dos cifras (2 a dos) aplicandole a cada combinacion todas las operaciones posibles
	for (int i = k; i < numerosCandidatos.size() && numerosCandidatos.size() < CIFRAS_MAXIMAS_ENCADENADAS; i++)
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

							//añadimos a numeros candidatos la nueva cifra, resultante de la op 
							numerosCandidatos.push_back(solParcial);

							//añadimos una posicion nueva al marcador para no repetir la nueva cifra al operar con ella en siguientes llamadas
							marcador.push_back(false);

							//rellenamos las estructuras que nos permitirá reconstruir la solucion
							ordenDeUso.push_back(numerosCandidatos[i]);
							ordenDeUso.push_back(numerosCandidatos[j]);
							operacionesEnOrden.push_back(operacion.simbolo);

							//nos guardamos los padres de la cifra que acabamos de añadir para reconstruir en la solucion solo las op extrictamente necesarias
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
							resolver(i, nivel + 1, solParcial, numerosCandidatos, ordenDeUso, operacionesEnOrden);

							//revertimos los marcadores a lavuelta de la recursiva porque no se siguen aplicando en este nivel, en orden segun los ultimos que hicimos
							//Así como las estructuras que nos ayudan a reconstruir la solucion
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
							 marcador[j] = false;

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
}*/

bool DFS::contained(vector<int> ordenDeUso, int cifra, int solMejor) {
	for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;

	//si la cifra no esta en orden de uso solo la mostraré si es la solucion
	return cifra == solMejor;
}
void DFS::mostrarDFS() {
	cout << "Numeros candidatos:( ";
	for (int i = 0; i < CIFRAS_INICIALES; i++)cout << numerosCandidatos[i] << " ";
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

