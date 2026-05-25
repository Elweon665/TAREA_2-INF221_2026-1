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
    
    long long mejor_resultado = greedy_1(m, e, lista_anime);
    
    cout << "La máxima satisfacción posible es: " << mejor_resultado << endl;

    return 0;
}