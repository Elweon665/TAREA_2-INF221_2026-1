#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

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

long long animaraton_fuerza_bruta(int index, int tiempo_restante, int energia_restante, const vector<Anime>& animes){

    if(index == animes.size()){
        return 0;
    }

    long long maxima_satiafaccion = animaraton_fuerza_bruta(index + 1, tiempo_restante, energia_restante, animes);

    int tiempo_acumulado = 0;
    int energia_acumulado = 0;
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

        maxima_satiafaccion = max(maxima_satiafaccion, satisfaccion_actual + satisfaccion_futura);
    }

    return maxima_satiafaccion;

}

int main(int argc, char* argv[]){
    string nombre_archivo;
    if (argc > 1) {
        nombre_archivo = argv[1]; 
    }
    else {
        cout << "Uso correcto: " << argv << " <archivo_de_prueba.txt>\n";
        cout << "Ejecutando con un nombre por defecto para pruebas..." << endl;
        nombre_archivo = "testcases_generados/testcases_3_1.txt"; // Cambia esto según necesites
    }

    ifstream archivo(nombre_archivo);

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombre_archivo << endl;
        return 1;
    }

    int n, m, e;
    archivo >> n >> m >> e;

    vector<Anime> lista_anime;
    lista_anime.reserve(n);

    for(int i = 0; i < n; ++i){
        Anime anime_actual;

        archivo >> anime_actual.nombre >> anime_actual.numero_caps >> anime_actual.bono;

        anime_actual.capitulos.reserve(anime_actual.numero_caps);

        for (int j = 0; j < anime_actual.numero_caps; ++j){
            Capitulo cap;

            archivo >> cap.tiempo >> cap.energia >> cap.satisfaccion;
            anime_actual.capitulos.push_back(cap);
        }
        lista_anime.push_back(anime_actual);
    }

    archivo.close();

    cout << "Procesando archivo: " << nombre_archivo << " (Animes: " << n << ")" << endl;
    
    long long mejor_resultado = animaraton_fuerza_bruta(0, m, e, lista_anime);
    
    cout << "La máxima satisfacción posible es: " << mejor_resultado << endl;

    return 0;
}

