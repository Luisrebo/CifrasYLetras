#include "DFS.h"
#include "Search.h"
#include <map>
#include <queue>
#include <iostream>



DFS::DFS(num_t nObjetivo, const std::array<num_t, CIFRAS_INICIALES>& numerosCandidatos) :Search(nObjetivo, numerosCandidatos), problemaBase(0, {}, {}, numerosCandidatos),
sol(problemaBase, 0, 0) {

}

SearchResult DFS::busqueda() {
	resolvere(0, problemaBase._numCandidatos, problemaBase._ordenDeUso, problemaBase._operacionesEnOrden);
	mostrarDFS();

	return SearchResult(sol._problema._operacionesEnOrden, sol._problema._ordenDeUso, sol._nivel, sol._solMejor);
	//limpiarSol();
}

void DFS::resolvere(int nivel, array<num_t, CIFRAS_INICIALES>& numCandidatos, array <num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordenDeUso, array <char, CIFRAS_INICIALES>& operacionesEnOrden) {

	//si llegamos a la mejor solucion nos detenemos

		//vamos a ir recorriendo los operandos para ir haciendo operaciones con las combinacones
		//recorremos los elementos de izq a dcha y para cada elemento operamos con los demas j=i+1
		//si no se pudiera hacer una operacion hacia un lado (i-j) probamos la operacion hacia el otro (j-1)
	for (int i = 0; i < CIFRAS_INICIALES - nivel - 1; ++i) {


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
}

bool DFS::contained(array<int, CIFRAS_INICIALES>& ordenDeUso, int cifra, int solMejor) {
	/*for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;

	//si la cifra no esta en orden de uso solo la mostraré si es la solucion
	return cifra == solMejor;*/
	for (int i = 0; i < ordenDeUso.size(); i++)
		if (ordenDeUso[i] == cifra) return true;
		else return false;
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
void DFS::limpiarSol() {

	//CONSTRUCCION DE LA ESTRUCTURA
	rastreoSolucion rastreo(sol._problema._operacionesEnOrden.size(), sol._problema._ordenDeUso.size());

	//CONSTRUIR EL MAPA	
	map<num_t, num_t> mapa;

	for (int i = 0; i < CIFRAS_INICIALES; ++i) {
		auto par = mapa.find(numerosCandidatos[i]);

		//si no existia aun el numero candidato enn el mapa 
		if (par == mapa.end())
			mapa.insert(std::make_pair(numerosCandidatos[i], 1));
		//si existia ya le añado uno
		else
			par->second++;
	}

	//cola de cifas que voy a rastrear fifo par<valor,posicion>
	queue<pair<num_t, num_t>> colaNumerosPorRastrear;

	//los indices comienzan en la ultima posicion del array y van marcando el comienzo de la solucion sin operaciones basura
	//inicialmente empiezo por rastrear los dos ultimos numeros que han formado la solucion
	/*colaNumerosPorRastrear.push({sol._problema._ordenDeUso[(sol._nivel * 2) + 1],(sol._nivel * 2) + 1});
	colaNumerosPorRastrear.push({ sol._problema._ordenDeUso[(sol._nivel * 2) ], (sol._nivel * 2)  });

	//agrego los dos ultimos numeros que formaron la solucion a la estructura que devolvere para reconstruir la solucion:
	rastreo._ordenDeUso[rastreo.indiceComienzoOrdenDeUso]= sol._problema._ordenDeUso[(sol._nivel * 2)+1];
	rastreo.indiceComienzoOrdenDeUso -= 1;

	rastreo._ordenDeUso[rastreo.indiceComienzoOrdenDeUso] = sol._problema._ordenDeUso[sol._nivel * 2];
	rastreo.indiceComienzoOrdenDeUso -= 1;

	//agrego tamb el simbolo de la operacion a la estructrura
	rastreo._operacionesEnOrden[rastreo.indiceComienzoOperacionesEnOrden]= sol._problema._operacionesEnOrden[sol._nivel];
	rastreo.indiceComienzoOperacionesEnOrden -= 1;*/

	colaNumerosPorRastrear.push({ sol._solMejor,sol._nivel * (2 + 1) });
	//para formar la solucion
	//vamos a buscar y acumular los numeros que forman una solucion lipia
	while (!colaNumerosPorRastrear.empty()) {
		auto par = colaNumerosPorRastrear.front();
		colaNumerosPorRastrear.pop();


		//las posiciones impares son el segundo operando de una operacion (ya que empezamos en cero y se emparejan dos a dos) por o tanto la cifra inmediatamente despues sera el operando con el que coabora y no
		// nos sirve para reconstruir una operacion que le haya dado fruto a el 

		//si estamos tratando con el segundo operando nos saltamos el primero  ej:5+6 si estamos viendo como se formo el 6 el 5 no nos interesa
		num_t indice = par.second;
		if (indice % 2 != 0)
			indice -= 1;

		//vamos recorriendo los numeros desde la posicion que ocupa hacia la izu¡quierda que es donde estarian las cifras quelo forman
		//todas las cifras estan formadas por otras dos de ordendeuso y estan colocadas de dos en dos desde la pos 0 por lo tanto podemos recorrerlas por pares
		for (int i = indice - 1; i >= 1; i -= 2) {

			num_t op1, op2;
			op1 = sol._problema._ordenDeUso[i];
			op2 = sol._problema._ordenDeUso[i - 1];

			//si las cifras que estoy recorriendo forman al numero que debia rastrear las añado a la estructura que representa la solucion
			//como estamos recorriendo la solucion al reves hay que tener cuidado con la divisio pues es op2 a la que le aplicamos la op1
			if (calcular(sol._problema._operacionesEnOrden[(indice / 2) - 1], op2, op1) == par.first) {

				//primero verifico si estamos ante un numero candidato y de serlo si puedo usarlo o si he de formarlo con otros numero candidatos por haberle usado antes
				auto parOp1 = mapa.find(op1);
				if (parOp1 != mapa.end() && parOp1->second > 0) {
					//actualizo para preferiblemente utilizar las cifras de cifras candidatas pero no excederme
					parOp1->second -= 1;
				}
				//si ya no me quedan cifras candidata de su magnitud o nunca lo fue tengo que construirla a partir de otras cifras
				else
					colaNumerosPorRastrear.push({ op1,indice });


				//lo añado a la estructura para reconstruir la solucion limpia
				rastreo._ordenDeUso[rastreo.indiceComienzoOrdenDeUso] = op1;
				rastreo.indiceComienzoOrdenDeUso -= 1;

				//La misma idea para op2
				auto parOp2 = mapa.find(op2);
				if (parOp2 != mapa.end() && parOp1->second > 0) {
					//actualizo para preferiblemente utilizar las cifras de cifras candidatas pero no excederme
					parOp2->second -= 1;
				}
				//si ya no me quedan cifras candidata de su magnitud o nunca lo fue tengo que construirla a partir de otras cifras
				else
					colaNumerosPorRastrear.push({ op2,indice - 1 });

				//lo añado a la estructura para reconstruir la solucion limpia
				rastreo._ordenDeUso[rastreo.indiceComienzoOrdenDeUso] = op2;
				rastreo.indiceComienzoOrdenDeUso -= 1;

				//guardamos tamb el simbolo de la operacion
				rastreo._operacionesEnOrden[rastreo.indiceComienzoOperacionesEnOrden] = sol._problema._operacionesEnOrden[indice / 2];
				rastreo.indiceComienzoOperacionesEnOrden -= 1;

				//salimos del bucle for no nos interesa seguir recorriendolo
				break;
			}

		}

	}

}

