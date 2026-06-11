import os
import re
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

entrada = "../data/measurements/resultados_generales.txt"
salida = "../data/plots"

#Reading a File in Python
#Python RegEx
#Ideas de Gemini
def extraer_datos(archivo):
    datos = []
    registro_actual = {}
    
    with open(archivo, 'r', encoding='utf-8') as f:
        for linea in f:
            linea = linea.strip()
            
            if linea.startswith("-----------------------------------"):
                if registro_actual:
                    datos.append(registro_actual)
                    registro_actual = {}
            elif linea.startswith('Dataset:'):
                nombre_dataset = linea.split('Dataset:')[1].strip()
                registro_actual['Dataset'] = nombre_dataset
                
                match_size = re.search(r'testcases_(\d+)', nombre_dataset)
                
                if match_size:
                    registro_actual['Size'] = int(match_size.group(1))
                else:
                    fallback_match = re.search(r'\d+', nombre_dataset)
                    registro_actual['Size'] = int(fallback_match.group()) if fallback_match else 0
         
            elif linea.startswith('Algoritmo:'):
                registro_actual['Algoritmo'] = linea.split('Algoritmo:')[1].strip()
            elif linea.startswith('Resultado (Satisfaccion):'):
                registro_actual['Resultado'] = float(linea.split(':')[1].strip())
            elif linea.startswith('Tiempo (ms):'):
                registro_actual['Tiempo_ms'] = float(linea.split('Tiempo (ms):')[1].strip())
            elif linea.startswith('Memoria utilizada (KB):'):
                registro_actual['Memoria_KB'] = float(linea.split('Memoria utilizada (KB):')[1].strip())
 
        if registro_actual:
            datos.append(registro_actual)
            
    return pd.DataFrame(datos)

#Graph Plotting in Python | Set 1
#Pyplot tutorial
#Ideas de Gemini
def generar_graficos():
    os.makedirs(salida, exist_ok=True)
    df = extraer_datos(entrada)

    if df.empty:
        print("NO hay datos en el archivo de mediciones")
        return

    df_optimo =df[df['Algoritmo'] == 'Dinamica'][['Dataset', 'Resultado']].rename(columns={'Resultado': 'Optimo'})
    df = df.merge(df_optimo, on='Dataset', how='left')
    df['Calidad_%'] = df.apply(lambda row: (row['Resultado'] / row['Optimo'] * 100) if row['Optimo'] > 0 else 0, axis=1)
    
    df_grouped = df.groupby(['Size', 'Algoritmo']).agg({
        'Tiempo_ms': 'mean',
        'Memoria_KB': 'mean',
        'Calidad_%': 'mean'
    }).reset_index()

    algoritmos = df_grouped['Algoritmo'].unique()

    for algo in algoritmos:
        if algo in ['Greedy_1', 'Greedy_2', 'Dinamica']:
            subset = df_grouped[df_grouped['Algoritmo'] == algo].sort_values('Size')
            if not subset.empty:
                plt.plot(subset['Size'], subset['Calidad_%'], marker='o', linewidth=2, label=algo)

        plt.title("Calidad de la Solución de las Heurísticas")
        plt.xlabel("Cantidad de animes (N)")
        plt.ylabel("Porcentaje respecto al Óptimo (%)")
        
        plt.axhline(100, color='red', linestyle='--', alpha=0.5, label='Optimo Ideal(100%)')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
        plt.tight_layout()
        if algo == 'Greedy_1':
            plt.savefig(os.path.join(salida, 'calidad_solucion_Greedy1.png'), dpi=300)
        elif algo == 'Greedy_2':
            plt.savefig(os.path.join(salida, 'calidad_solucion_Greedy2.png'), dpi=300)
        plt.close()

        plt.figure(figsize=(10, 6))
        for algo in algoritmos:
            subset = df_grouped[df_grouped['Algoritmo'] == algo].sort_values('Size')
            if not subset.empty:
                plt.plot(subset['Size'], subset['Tiempo_ms'], marker='o', linewidth=2, label=algo)
        plt.title("Tiempo de ejecucion Promedio")
        plt.xlabel("Cantidad de animes (N)")
        plt.ylabel("Tiempo (ms)")
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(salida, 'tiempo_ejecucion.png'), dpi=300)
        plt.close()
        
        plt.figure(figsize=(10, 6))
        for algo in algoritmos:
            subset = df_grouped[df_grouped['Algoritmo'] == algo].sort_values('Size')
            if not subset.empty:
                plt.plot(subset['Size'], subset['Memoria_KB'], marker='s', linewidth=2, linestyle='-.', label=algo)

        plt.title("Consumo de Memoria Promedio")
        plt.xlabel("Cantidad de animes (N)")
        plt.ylabel("Memoria Promedio Utilizada (KB)")
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(salida, 'consumo_memoria.png'), dpi=300)
        plt.close()

        print("Graficos generados con exito, revise en la carpeta data/plots")

if __name__ == "__main__":
    generar_graficos()