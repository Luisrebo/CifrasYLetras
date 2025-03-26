
#include "FuncionesComunes.h"

num_t calcular(char operacion, num_t solucionParcial, num_t operando) {
	switch (operacion) {
	case '+':
		return solucionParcial + operando;
	case '-':
		return solucionParcial - operando;
	case '*':
		return solucionParcial * operando;
	case '/':
		if (operando != 0) {
			return solucionParcial / operando;
		}
		else {
			return 0; // Evitar división por cero
		}
	default:
		return solucionParcial;
	}
}
SearchResult::SearchResult(const std::array<char, CIFRAS_INICIALES>& operacionesEnOrden,
	const std::array<num_t, CIFRAS_MAXIMAS_ENCADENADAS>& ordendeUso,
	num_t nivel, num_t mejorSol) :_mejorSol(mejorSol)
{
	num_t nNum = 2 * (nivel + 1);
	// Asegurarse de no copiar más elementos de los que existen en cada array.
	if (nivel > CIFRAS_INICIALES)
		nivel = CIFRAS_INICIALES;
	if (2 * nNum > CIFRAS_MAXIMAS_ENCADENADAS)
		nNum = CIFRAS_MAXIMAS_ENCADENADAS;

	// Copiar los primeros nOperaciones elementos del array de char.
	_operacionesEnOrden.assign(operacionesEnOrden.begin(), operacionesEnOrden.begin() + nivel + 1);

	// Copiar los primeros nNum elementos del array de num_t.
	_ordendeUso.assign(ordendeUso.begin(), ordendeUso.begin() + nNum);
}