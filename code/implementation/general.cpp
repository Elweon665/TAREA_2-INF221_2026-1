/*Fuentes de informacion:
-C++ Files and Streams https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
-How to Measure Memory Usage in C++ https://mrswolf.github.io/memory-usage-cpp/
-Using getline() with the file input in C++ https://stackoverflow.com/questions/20739453/using-getline-with-file-input-in-c
-Getline en C++ - Ejemplo con función getline() https://www.freecodecamp.org/espanol/news/getline-en-c-ejemplo-con-funcion-getline/
-getrusage(2) — Linux manual page https://man7.org/linux/man-pages/man2/getrusage.2.html
-Ideas de gemini www.gemini.google.com
-The /proc Filesystem https://www.kernel.org/doc/html/latest/filesystems/proc.html
-std::filesystem::path https://en.cppreference.com/w/cpp/filesystem/path.html*/


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

//C++ Files and Streams https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
//Ciertas ideas del codigo fueron tomados de aqui
void writeResultToFile(long long resultado, const string& nombre_de_archivo){
    ofstream file(nombre_de_archivo);
    if(file.is_open()){
        file << resultado << "\n";
    }
}

//Ideas de gemini
//The /proc Filesystem
//Se combinaronambas ideas para crear la funcion
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

//-C++ Files and Streams
//Using getline() with the file input in C++
//Se cominaron ambas fuentes ppara la funcion.
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

//-C++ Files and Streams
//Using getline() with the file input in C++
//Ideas de Gemini

void realizar_mediciones(function<long long ()> sortAlgorithm, const string& algoName, const string& datasetName){
    long memoria_inicial = get_MemoryUsage();

    auto start = high_resolution_clock::now();
    
    long long resultado_optimo = sortAlgorithm(); 
    
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

//-C++ Files and Streams
//Using getline() with the file input in C++
//Ideas de Gemini
//std::filesystem::path
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
                cout << "  -> Saltando Fuerza Bruta (n > 40, ha sido probado antes y toma muchas horas en ejecutar)..." << endl;
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