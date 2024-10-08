/*@ <authors>
 *
 * Nombre, apellidos y usuario del juez (TAISXX) de los autores de la solución.
 *
 *@ </authors> */

#include <iostream>
#include <fstream>
#include <vector>
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

//En este struct vamos a guardar las soluciones
struct Solucion {
    vector<int> ordenDeUso;
    vector<string> operacionesEnOrden;
    int solMejor;


};


//En cada operacion (suma resta division multiplicacion saco y meto los mismo numeros al vector y actualizo los marcadores)
//no hace falta hacerlo todo el rato de hecho es ineficiente pero es una primera version muy bruta en la que no quiero refactorizar nada

int recursivoBruto(int nivel, int solParcial, int& mejorSol, vector<bool>& marcador, const int& numObjetivo, const vector<int>& numerosCandidatos, vector<int> ordenDeUso, vector<string> operacionesEnOrden, Solucion& sol) {

    if (nivel >= numerosCandidatos.size())
        return solParcial;

    if (solParcial == numObjetivo) {
        //mejorSol = solParcial;//estaba ya actualizada
        return solParcial;
    }

    //recorro todos los candidatos (puedo utilizarlo o no)
    for (int i = 0; i < numerosCandidatos.size(); i++) {

        //en el primer nivel solo podemos utilizar la operacion suma asique la haremos siempre
        if (!marcador[i]) {
            //SUMA

            //actualizamos la sol parcial
            solParcial += numerosCandidatos[i];

            //Nos guardamos en orden los numeros segun los vamos usando para recomponer la solucion
            ordenDeUso.push_back(numerosCandidatos[i]);

            //guardamos la operacion que aplicamos a la solParcial con el numero i (al principio siempre es una suma )
            operacionesEnOrden.push_back("+");

            //marcamos el numero i para no repetirlo
            marcador[i] = true;

            //si tenemos una solucion mas cercana al numero objetivo sin excederlo
            if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial>0) {
                mejorSol = solParcial;
                sol.operacionesEnOrden = operacionesEnOrden;
                sol.ordenDeUso = ordenDeUso;
                sol.solMejor = mejorSol;
            }//nos guardamos aqui los array para reconstruir?
           
            recursivoBruto(nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

            //desmarcamos
            marcador[i] = false;

            //borramos para guardarnos soluciones parciales distintas
            operacionesEnOrden.pop_back();

            //borramos el numero introducido para ir guardando otras combinaciones
            ordenDeUso.pop_back();

            solParcial -= numerosCandidatos[i];

            //si no estamos en el nivel 0 podemos tambien restar multiplicar y dividir ya habiendo sumado pues se hace siempre que el numero no este repetido
            if (nivel != 0) {

                // ¿¿tenemos en cuenta soluciones trianguladas ? 10 = (8 - 3) * 2  Y 5 * 2->candidatos = { 8,3,5,2 } numObj = 10 ??

                //RESTA
               //si sale positivo
                if (solParcial - numerosCandidatos[i] > 0) {

                    solParcial -= numerosCandidatos[i];
                    ordenDeUso.push_back(numerosCandidatos[i]);
                    operacionesEnOrden.push_back("-");
                    marcador[i] = true;

                    //actualizamos sol
                    if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial >= 0) {
                        mejorSol = solParcial;
                        sol.operacionesEnOrden = operacionesEnOrden;
                        sol.ordenDeUso = ordenDeUso;
                        sol.solMejor = mejorSol;
                    }//nos guardamos aqui los array para reconstruir?
                  
                    recursivoBruto(nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

                    solParcial += numerosCandidatos[i];
                    ordenDeUso.pop_back();
                    operacionesEnOrden.pop_back();
                    marcador[i] = false;

                }


                //MULTIPLICACION

                solParcial *= numerosCandidatos[i];
                ordenDeUso.push_back(numerosCandidatos[i]);
                operacionesEnOrden.push_back("*");
                marcador[i] = true;

                //actualizamos sol
                if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial>0) {
                    mejorSol = solParcial;
                    sol.operacionesEnOrden = operacionesEnOrden;
                    sol.ordenDeUso = ordenDeUso;
                    sol.solMejor = mejorSol;
                }//nos guardamos aqui los array para reconstruir?             

                recursivoBruto(nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

                solParcial /= numerosCandidatos[i];
                ordenDeUso.pop_back();
                operacionesEnOrden.pop_back();
                marcador[i] = false;

                //DIVISION

                //si es divisible
                if (solParcial % numerosCandidatos[i] == 0) {

                    solParcial /= numerosCandidatos[i];
                    ordenDeUso.push_back(numerosCandidatos[i]);
                    operacionesEnOrden.push_back("/");
                    marcador[i] = true;

                    //actualizamos sol
                    if (numObjetivo - solParcial < numObjetivo - mejorSol && solParcial >= 0 && numObjetivo - solParcial>0) {

                        mejorSol = solParcial;
                        sol.operacionesEnOrden = operacionesEnOrden;
                        sol.ordenDeUso = ordenDeUso;
                        sol.solMejor = mejorSol;
                    }//nos guardamos aqui los array para reconstruir?
                 
                    recursivoBruto(nivel + 1, solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

                    solParcial *= numerosCandidatos[i];
                    ordenDeUso.pop_back();
                    operacionesEnOrden.pop_back();
                    marcador[i] = false;

                }

                //SALTAR No utilizamos el numero i
               /* marcador[i] = true;
                recursivoBruto(nivel , solParcial, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden,sol);
                marcador[i] = false;*/
            }
        }
    }
}
int calcular(string operacion, int solucionparcial, int operando) {
    if (operacion == "+") {
        return solucionparcial += operando; // Sumar el operando al resultado
    }
    else if (operacion == "-") {
        return solucionparcial -= operando; // Resta (puedes definir cómo manejarlo)
    }
    else if (operacion == "*") {
        return solucionparcial *= operando; // Multiplicación (inicializa el resultado adecuadamente)
    }
    else if (operacion == "/") {

        return solucionparcial /= operando;
    }
}

void resuelveCaso() {

    //numero a alcanzar entre 101 y 999
   // int numObjetivo = 872; // 7+7=14 14*8=112 112+50=162 162*6=972 972-100= 872
    int numObjetivo;

    vector<int> numerosCandidatos(6);

    cin >> numObjetivo;

    for (int i = 0; i < 6; i++)
        cin>>numerosCandidatos[i];
    /* numerosCandidatos.push_back(8);
     numerosCandidatos.push_back(7);
     numerosCandidatos.push_back(6);
     numerosCandidatos.push_back(50);
     numerosCandidatos.push_back(100);
     numerosCandidatos.push_back(7);*/

     //NO HAY EXACTO
    /* int numObjetivo = 902;
     numerosCandidatos.push_back(25);
     numerosCandidatos.push_back(7);
     numerosCandidatos.push_back(10);
     numerosCandidatos.push_back(1);
     numerosCandidatos.push_back(9);
     numerosCandidatos.push_back(1);*/

     //lo hace bien pero existe uno con menos operaciones 50+10=60,60-4,56*6,336+5=341
  /*  int numObjetivo = 341;
    numerosCandidatos.push_back(6);
    numerosCandidatos.push_back(50);
    numerosCandidatos.push_back(3);
    numerosCandidatos.push_back(10);
    numerosCandidatos.push_back(5);
    numerosCandidatos.push_back(4);*/

    vector<bool> marcador(6);
    vector<string> operacionesEnOrden;
    vector<int> ordenDeUso;
    Solucion sol;
    sol.solMejor = 0;
    int mejorSol = 0;

    // resolver el caso posiblemente llamando a otras funciones

    // escribir la solución
    recursivoBruto(0, 0, mejorSol, marcador, numObjetivo, numerosCandidatos, ordenDeUso, operacionesEnOrden, sol);

    cout << "Numero Objetivo:" << numObjetivo<< " Solucion:" << sol.solMejor;
    if (sol.solMejor != numObjetivo)cout << "\n"<<"NO HAY SOLUCION EXACTA";
    cout<< "\n";

    //para llevar la cuenta
    int solu;
    for (int i = 0; i < sol.ordenDeUso.size(); i++) {
        if (i == 0) {
        cout << sol.ordenDeUso[i];
        solu = calcular(sol.operacionesEnOrden[i], 0, sol.ordenDeUso[i]);
    }
        else {
            solu = calcular(sol.operacionesEnOrden[i], solu, sol.ordenDeUso[i]);
            cout << sol.operacionesEnOrden[i] << sol.ordenDeUso[i] <<"="<<solu << "\n";
           
            cout << solu ;
        }
    }
    cout << "\n";
}

//@ </answer>
//  Lo que se escriba dejado de esta línea ya no forma parte de la solución.

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("Texto.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}
