/* Fuentes de informacion:
-Header Files in C++ https://www.geeksforgeeks.org/cpp/header-files-in-c-c-with-examples/ */


//Header Files in C++
#ifndef GENERAL_H
#define GENERAL_H

#include <vector>
#include <string>

using namespace std;

struct Capitulo{
    int tiempo;
    int energia;
    long long satisfaccion;
};

struct Anime{
    string nombre;
    int numero_caps;
    long long bono;
    vector<Capitulo> capitulos;
};

//Header Files in C++
long long animaraton_fuerza_bruta(int index, int tiempo_restante, int energia_restante, const vector<Anime>& animes);
long long greedy_1(int m, int e, const vector<Anime>& animes);
long long greedy_2(int m, int e, const vector<Anime>& animes);
long long dinamica(int M, int E, const vector <Anime>& animes);

#endif