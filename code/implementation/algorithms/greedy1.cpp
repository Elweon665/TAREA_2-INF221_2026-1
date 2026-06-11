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


long long greedy_1(int m, int e, const vector<Anime>& animes){
    int n = animes.size();

    vector<int> progreso(n, 0);
    vector<bool> elegible(n, true);

    long long satisfaccion_total = 0;
    int tiempo_restante = m;
    int energia_restante = e;

    while(true){
        int mejor_anime = -1;
        double mejor_ratio = -1.0;

        for(int i = 0; i < n; i++){
            if(!elegible[i]) continue;

            int cap_idx = progreso[i];
            const Capitulo& cap_actual = animes[i].capitulos[cap_idx];

            if(cap_actual.tiempo <= tiempo_restante && cap_actual.energia <= energia_restante) {


                double costo_combinado= (double)cap_actual.tiempo + cap_actual.energia;
                double ratio = (double)cap_actual.satisfaccion / costo_combinado;

                if(ratio > mejor_ratio){
                    mejor_ratio = ratio;
                    mejor_anime = i;
                }
            } else {
                elegible[i] = false;
            }
        }
        if(mejor_anime == -1){
            break;
        }

        int cap_idx = progreso[mejor_anime];
        const Capitulo&  cap_elegido = animes[mejor_anime].capitulos[cap_idx];

        tiempo_restante -= cap_elegido.tiempo;
        energia_restante -= cap_elegido.energia;
        satisfaccion_total += cap_elegido.satisfaccion;

        progreso[mejor_anime]++;

        if(progreso[mejor_anime] == animes[mejor_anime].numero_caps){
            satisfaccion_total += animes[mejor_anime].bono;
            elegible[mejor_anime] = false;
        }


    }

    return satisfaccion_total;

}
