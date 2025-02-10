
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