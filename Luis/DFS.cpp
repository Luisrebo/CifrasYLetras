#include "DFS.h"
#include "Search.h"
#include <iostream>



DFS::DFS(int nObjetivo,  vector<int>  &nCandidatos) :Search(nObjetivo, numerosCandidatos), mejorSol(0), marcador(6, false), ordenDeUso(0), operacionesEnOrden(), numerosCandidatos(nCandidatos), numObjetivo(nObjetivo) {

	//sol.solMejor = 0;
	//resolver(0, 0, 0, mejorSol, marcador, nObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

}

void DFS::busqueda() {
	sol.solMejor = 0;
	resolver(0, 0, 0/*, marcador*/, numerosCandidatos, ordenDeUso, operacionesEnOrden);
	mostrarDFS();
}

//funcion recursiva 
//void DFS::resolver(int k, int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int numObjetivo, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden, SolucionDFS& sol) {
void DFS::resolver(int k, int nivel, int solParcial /* ,int& mejorSol*//*, vector<bool>& marcador*/ /*, const int numObjetivo*/, vector<int>& numerosCandidatos, vector<int>& ordenDeUso, vector<char>& operacionesEnOrden /*, SolucionDFS& sol*/) {
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
							resolver(i, nivel + 1, solParcial/*, marcador*/, numerosCandidatos, ordenDeUso, operacionesEnOrden);

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

bool DFS::contained(vector<int> ordenDeUso, int cifra, int solMejor) {
	for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;

	//si la cifra no esta en orden de uso solo la mostraré si es la solucion
	return cifra == solMejor;
}

void DFS::mostrarDFS() {
	cout << "Numeros candidatos:( ";
	for (int i = 0; i < numerosCandidatos.size(); i++)cout << numerosCandidatos[i] << " ";
	cout << ")" << "\n";

	cout << "Numero Objetivo:" << numObjetivo << " Solucion:" << sol.solMejor;
	if (sol.solMejor != numObjetivo)cout << "\n" << "NO HAY SOLUCION EXACTA";
	cout << "\n";


	for (int i = 0, j = 0; i < sol.ordenDeUso.size() - 1; i += 2, j++) {
		//solo muestro el numero si esta contenido en las operaciones que he utilizado para desarrollar la solucion
		if (contained(sol.ordenDeUso, calcular(sol.operacionesEnOrden[j], sol.ordenDeUso[i], sol.ordenDeUso[i + 1]), sol.solMejor))
			cout << sol.ordenDeUso[i] << sol.operacionesEnOrden[j] << sol.ordenDeUso[i + 1] << "=" << calcular(sol.operacionesEnOrden[j], sol.ordenDeUso[i], sol.ordenDeUso[i + 1]) << '\n';
	}
	cout << "\n";
}