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

struct AnimeRatio {
    int indice_original;
    double ratio_global;
};

long long greedy_2(int m, int e, const vector<Anime>& animes){
    int n = animes.size();

    vector<AnimeRatio> ratios(n);

    for(int i = 0; i<n; i++){
        long long satisfaccion_total_anime = animes[i].bono;
        int costo_tiempo_anime = 0;
        int costo_energia_anime = 0;

        for(const auto& cap : animes[i].capitulos){
            satisfaccion_total_anime += cap.satisfaccion;
            costo_tiempo_anime += cap.tiempo;
            costo_energia_anime += cap.energia;
        }

        double costo_combinado = (double)costo_tiempo_anime + costo_energia_anime;

        if (costo_combinado == 0) costo_combinado = 1.0;

        ratios[i].indice_original = i;
        ratios[i].ratio_global = (double)satisfaccion_total_anime / costo_combinado;
    }

    sort(ratios.begin(), ratios.end(), [](const AnimeRatio& a, const AnimeRatio& b){
        return a.ratio_global > b.ratio_global;
    });

    long long satisfaccion_total = 0;
    int tiempo_restante = m;
    int energia_restante = e;

    for(int i = 0; i < n; i++){
        int idx = ratios[i].indice_original;
        const Anime& anime_actual = animes[i];

        int caps_vistos = 0;

        for (const auto& cap : anime_actual.capitulos){
            if (cap.tiempo <= tiempo_restante && cap.energia <= energia_restante){
                tiempo_restante -= cap.tiempo;
                energia_restante -= cap.energia;
                satisfaccion_total += cap.satisfaccion;
                caps_vistos++;
            } else {
                break;
            }
        }
        if(caps_vistos == anime_actual.numero_caps){
            satisfaccion_total += anime_actual.bono;
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
    
    long long mejor_resultado = greedy_2(m, e, lista_anime);
    
    cout << "La máxima satisfacción posible es: " << mejor_resultado << endl;

    return 0;
}