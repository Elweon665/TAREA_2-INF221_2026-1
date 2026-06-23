/* Fuentes de informacion:
-Recursive Knapsack 0-1 Problem:https://www.geeksforgeeks.org/0-1-knapsack-problem-dp-10
-Ideas de gemini*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include "general.h"

using namespace std;
namespace fs = std::filesystem;

//-Recursive Knapsack 0-1 Problem: Se tomo parte de la idea del problema de mochila para realizar una parte del codigo.
//Ideas e Gemini: Se utilizó para ayudar a adaptar dicho problema al contexto de la tarea, manteniendo el enfoque de fuerza bruta.
long long animaraton_fuerza_bruta(int index, int tiempo_restante, int energia_restante, const vector<Anime>& animes){

    if(index == animes.size()){
        return 0;
    }

    long long maxima_satisfaccion = animaraton_fuerza_bruta(index + 1, tiempo_restante, energia_restante, animes);

    long long tiempo_acumulado = 0;
    long long energia_acumulado = 0;
    long long satisfaccion_acumulada = 0;

    for(int k = 0; k < animes[index].numero_caps; ++k){
        tiempo_acumulado += animes[index].capitulos[k].tiempo;
        energia_acumulado += animes[index].capitulos[k].energia;
        satisfaccion_acumulada += animes[index].capitulos[k].satisfaccion;

        if(tiempo_acumulado > tiempo_restante || energia_acumulado > energia_restante){
            break;
        }

        long long satisfaccion_actual = satisfaccion_acumulada;

        if (k == animes[index].numero_caps - 1){
            satisfaccion_actual += animes[index].bono;
        }

        long long satisfaccion_futura = animaraton_fuerza_bruta(
            index + 1, 
            tiempo_restante - tiempo_acumulado, 
            energia_restante - energia_acumulado, 
            animes
        );

        maxima_satisfaccion = max(maxima_satisfaccion, satisfaccion_actual + satisfaccion_futura);
    }

    return maxima_satisfaccion;

}
