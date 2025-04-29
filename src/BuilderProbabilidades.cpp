#include "BuilderProbabilidades.h"

BuilderProbabilidades::BuilderProbabilidades() {}

void BuilderProbabilidades::update(const string& palabraDelDiccionario) {

	//Vamos a sacar probabilidades y estadisticas de las palabras de nuestro vocabulario para guiar las busquedas de soluciones mas tarde

	//recorremos cara caracter de la palabra
	for (int i = 0; i < palabraDelDiccionario.length(); ++i) {

		//establecemos el indice que tendra ese caracter en los arrys en funcion al char que represente
		//donde la letra a estaria en la pos 0 la b en la 1 .. la ñ ={ en la 26
		int indiceCharEnArray = palabraDelDiccionario[i] - 'a';

		//llevamos la cuenta de las veces que aparece cada letra
		leterCounts[indiceCharEnArray].first += 1;
		leterCounts[indiceCharEnArray].second = palabraDelDiccionario[i];//redundante

		//Vamos a actualizar la probabilidad de que una letra salga despues de otra
		//para la letra palabraDelDiccionario[i], si no es la ultima de la palabra, vamos a registrar cual es la siguiente que saldra para llevar una probabilidad a futuro de a partir de 
		//una letra cual es la mas probable que salga a continuacion
		if (i < palabraDelDiccionario.length() - 1) {

			//indice a patir de su char de laletra consecutiva a la actual
			int indiceCharConsecutivoEnArray = palabraDelDiccionario[i + 1] - 'a';
			probabilidades.matricesDeProbabilidades.probabilidadLetraPosterior[indiceCharEnArray][indiceCharConsecutivoEnArray] += 1;

			//ahora aumentamos el contador del numero de letras que salieron detras de la letra i para poder dividir al hacer la probabilidad
			probabilidades.sumaTotalDeProbabilidadesPorFilasLetraPosterior[indiceCharEnArray] += 1;

		}

		//Ahora llevamos un contador de las veces que aparecio la letra palabraDelDiccionario[i] en el nivel i
		//y del numero de letras que aparecioren en el nivel i para hacer estadistica 
		//Notese que en nuestro arbol el nivel 0 pertenece a la raiz y tenemos que tener en cuenta que al no dejar una fila libre en la matriz
		//realmente estamos representanto para la fila i el nivel i+1 ya que en el nivel 0 del trie siempre estara la raiz y solo la raiz
		//lo tenemos en cuenta para implementarlo en el comparador
		probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel[i][indiceCharEnArray] += 1;
		probabilidades.sumaTotalDeProbabilidadesPorFilasLetraPorNivel[i] += 1;
	}
}

//una vez leidas todas las palabras de nuestro diccinario calcullamos las probabilidades spbre los conteos realizados
void BuilderProbabilidades::calcularProbabilidades() {

	//para no dividir entre cero declaramos un numero muy pequeño, cercano a cero pero que no produce error
	double evitarDivisionEntreCero = 1e-5;

	//recorremos la matriz para calcular la probabilidad que hay de que salga una letra j en un nivel i,
	//dividimos las veces que salio la letra j en el nivel i / las veces que salio cualquier letra en se nivel (suma de la fila entera, guardado en el array del struct probabilidades)
	for (int i = 0; probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel.size(); ++i) {

		for (int j = 0; j < probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel[i].size(); ++j) {

			probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel[i][j] =
				probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel[i][j] /
					 max(evitarDivisionEntreCero, probabilidades.sumaTotalDeProbabilidadesPorFilasLetraPorNivel[i]);
		}
	}

	//recorremos la matriz para calcular la probabilidad de que salga la letra j justo despues de la letra i
	//dividimos las veces que salio la letra j despues de la letra i / las veces que salio cualquier letra despues de la letra i (suma de la fila entera, guardado en struct probabilidades)
	for (int i = 0; i < probabilidades.matricesDeProbabilidades.probabilidadLetraPosterior.size(); ++i) {

		for (int j = 0; j < probabilidades.matricesDeProbabilidades.probabilidadLetraPorNivel[i].size(); ++j) {

			probabilidades.matricesDeProbabilidades.probabilidadLetraPosterior[i][j] =
				probabilidades.matricesDeProbabilidades.probabilidadLetraPosterior[i][j] /
					max(probabilidades.sumaTotalDeProbabilidadesPorFilasLetraPosterior[i], evitarDivisionEntreCero);
		}
	}
}

MatricesDeProbabilidades BuilderProbabilidades::getProbabilidades() {

	return probabilidades.matricesDeProbabilidades;
}
