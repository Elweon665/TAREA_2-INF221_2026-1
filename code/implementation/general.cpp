#include <bits/stdc++.h>
#include <filesystem>
#include <chrono>
#include <sys/resource.h>
#include <functional>
#include <iostream>
#include "algorithms/general.h"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

void writeResultToFile(long long resultado, const string& nombre_de_archivo){
    ofstream file(nombre_de_archivo);
    if(file.is_open()){
        file << resultado << "\n";
    }
}

int get_MemoryUsage(){
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            istringstream iss(line);
            string key;
            long value;
            string unit;
            iss >> key >> value >> unit; 
            return value;
        }
    }
    return 0;
}

void Leer_Caso(const string& filepath, int& m, int& e, vector<Anime>& animes){
    ifstream archivo(filepath);
    if (!archivo.is_open()) {
        cerr << "Error al abrir: " << filepath << endl;
        return;
    }
    int n;
    archivo >> n >> m >> e;

    animes.clear();
    animes.reserve(n);
    for(int i = 0; i < n; ++i){
        Anime anime_actual;
        archivo >> anime_actual.nombre >> anime_actual.numero_caps >> anime_actual.bono;
        anime_actual.capitulos.reserve(anime_actual.numero_caps);
        for(int j = 0; j < anime_actual.numero_caps; ++j){
            Capitulo cap;
            archivo >> cap.tiempo >> cap.energia >> cap.satisfaccion;
            anime_actual.capitulos.push_back(cap);
        }
        animes.push_back(anime_actual);
    }
    archivo.close();
}

void realizar_mediciones(function<long long ()> sortAlgorithm, const string& algoName, const string& datasetName){
    long memoria_inicial = get_MemoryUsage();

    auto start = high_resolution_clock::now();
    
    // Ejecutar el algoritmo y guardar el resultado
    long long resultado_optimo = sortAlgorithm(); 
    
    // Nota: en recursiones profundas, el peak real puede ocurrir en medio, 
    // pero leer VmRSS aquí sigue siendo la métrica de asignación más confiable
    long memoria_post = get_MemoryUsage(); 
    auto stop = high_resolution_clock::now();

    long memoria_usada = max(0L, memoria_post - memoria_inicial);
    auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    fs::create_directories("data/measurements");
    string outFilename = "data/measurements/resultados_generales.txt";
    ofstream outFile(outFilename, ios::app);

    if(outFile.is_open()) {
        outFile << "Dataset: " << datasetName << "\n";
        outFile << "Algoritmo: " << algoName << "\n";
        outFile << "Tiempo (ms): " << duracion.count() << "\n";
        outFile << "Resultado (Satisfaccion): " << resultado_optimo << "\n";
        outFile << "Memoria utilizada (KB): " << memoria_usada << "\n";
        outFile << "-----------------------------------\n";
        outFile.close();
    } else {
        cerr << "Error al abrir archivo de salida para " << algoName << endl;
    }
    fs::create_directories("data/outputs");
    string sortedFilename = "data/outputs/" + algoName + "_" + datasetName + "_out.txt";
    writeResultToFile(resultado_optimo, sortedFilename);
}

int main(){
    string directorio_entrada = "data/inputs";

    fs::create_directories("data/measurements");
    fs::create_directories("data/outputs");
    ofstream limpiarArchivo("data/measurements/resultados_generales.txt", ios::trunc);
    limpiarArchivo.close();
    
    cout << "Iniciando benchmarks AniMaraton..." << endl;

    for (const auto& entry : fs::directory_iterator(directorio_entrada)){
        if (entry.is_regular_file() && entry.path().extension() == ".txt"){
            string filepath = entry.path().string(); 
            string filename = entry.path().filename().string(); 
            string datasetName = entry.path().stem().string();

            cout << "\nProcesando caso: " << datasetName << endl;

            int M, E;
            vector<Anime> lista_animes;
            Leer_Caso(filepath, M, E, lista_animes);

            if (lista_animes.size() <= 40) {
                cout << "  -> Ejecutando Fuerza Bruta..." << endl;
                realizar_mediciones(
                    [&]() { return animaraton_fuerza_bruta(0, M, E, lista_animes); }, 
                    "FuerzaBruta", 
                    datasetName
                );
            } else {
                cout << "  -> Saltando Fuerza Bruta (n > 40, tardaria años)..." << endl;
            }

            cout << " Ejecutando Greedy 1 " << endl;
            realizar_mediciones([&]() {return greedy_1(M, E, lista_animes); }, "Greedy_1", datasetName);
            cout << " Ejecutando Greedy 2 " << endl;
            realizar_mediciones([&]() {return greedy_2(M, E, lista_animes); }, "Greedy_2", datasetName);
            cout << " Ejecutando Programacion dinamica " << endl;
            realizar_mediciones([&]() {return dinamica(M, E, lista_animes); }, "Dinamica", datasetName);

        }

    }

    cout << "Resultados guardados!" << endl;
    cout << "Generando graficos..." << endl;
    system("cd scripts && python3 plot_generator.py"); 

    return 0;

}