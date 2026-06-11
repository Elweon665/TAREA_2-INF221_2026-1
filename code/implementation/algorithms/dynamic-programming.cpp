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


long long dinamica(int M, int E, const vector <Anime>& animes){
    vector<vector<long long>> dp(M + 1, vector<long long>(E +1,0));

    struct OpcionPrefijo{
        int tiempo_total;
        int energia_total;
        long long satisfaccion_total;
    };

    for (const Anime& anime : animes){
        vector<OpcionPrefijo> opciones;

        int tiempo_acumulado = 0;
        int energia_acumulada = 0;
        long long satisfaccion_acumulada = 0;

        for(int j = 0; j < anime.numero_caps; ++j){
            tiempo_acumulado += anime.capitulos[j].tiempo;
            energia_acumulada += anime.capitulos[j].energia;
            satisfaccion_acumulada += anime.capitulos[j].satisfaccion;

            long long satisfaccion_final = satisfaccion_acumulada;

            if(j == anime.numero_caps - 1){
                satisfaccion_final += anime.bono;
            }

            opciones.push_back({tiempo_acumulado, energia_acumulada, satisfaccion_final});
        }

        vector <vector<long long>> next_dp = dp;

        for(int m = 0; m <= M; ++m){
            for (int e = 0; e <= E; ++e){
                for(const auto& op:opciones){
                    if(m >= op.tiempo_total && e >= op.energia_total){
                        next_dp[m][e] = max(next_dp[m][e], dp[m - op.tiempo_total][e - op.energia_total] + op.satisfaccion_total);
                    }
                }
            }
        }
        dp = next_dp;
    }
    return dp[M][E];
}