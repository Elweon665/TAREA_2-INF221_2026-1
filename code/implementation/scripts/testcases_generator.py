"""
Este codigo debe generar casos de prueba para los algoritmos implementados en C++, en la carpeta code/implementation/data/intputs/
"""

import numpy as np
import os
import random
from string import ascii_uppercase, ascii_lowercase

def generar_casos(n, identifier, output_dir="."):
    m = random.randint(1, 3000)
    e = random. randint(1, 500)

    q_values = []
    q_total = 0
    for _ in range(n):
        max_q = min(30, 700 - q_total - (n - len(q_values)-1))
        if max_q < 1:
            max_q = 1
        
        q_i = random.randint(1, max(1, min(10, max_q)))
        q_values.append(q_i)
        q_total += q_i
    filename = os.path.join(output_dir, f"testcases_{n}_{identifier}.txt")
    with open(filename, 'w') as f:
        f.write(f"{n} {m} {e}\n")

        for i in range(n):
            anime_name = f"anime_{i+1}_gen"
            q_i = q_values[i]
            b_i = random.randint(0, 1000000000)

            f.write(f"{anime_name} {q_i} {b_i}\n")

            for _ in range(q_i):
                t_ij = random.randint(1, 300)
                c_ij = random.randint(1, 100)
                v_ij = random.randint(1, 1000000000)

                f.write(f"{t_ij} {c_ij} {v_ij}\n")
    print(f"Generado: {filename} (Capítulos totales: {q_total}, Minutos: {m}, Energía: {e})")

def main():
    casos_pequenos = [3,5,8]
    casos_medianos = [20, 40, 80]
    casos_grandes = [100, 150, 200]

    total_tamanos = casos_pequenos + casos_medianos + casos_grandes

    output_dir = "../data/inputs"
    os.makedirs(output_dir, exist_ok=True)

    for n in total_tamanos:
        for i in range(1,4):
            generar_casos(n, i , output_dir)

if __name__ == "__main__":
    main()