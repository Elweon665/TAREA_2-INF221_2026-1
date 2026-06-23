# Documentación

"¿Quien ganará? Greedys vs Dinámico vs Bruto." 
Nombre: Vicente Carvajal Helqui Rol: 202273572-4

En esta entrega se estudia el comportamiento de los algoritmos de fuerza bruta, heuristicas de greedy y programacion dinamica a la hora de obtener la satisfaccion maxima a la hora de ver n cantidad de animes en base a distintas restricciones

Explicaciones de ejecucion de codigo: Diríjase a la carpeta code/implementation en la terminal, dentro de ella escriba: "make run". y el codigo general.cpp se ejecutará, y al terminar su ejecucion llamará al script "plot_generator.py" ubicado en la carpeta "scripts".

En caso de querer solamente graficar, diríjase a la carpeta "scripts" y ejecute plot_generator.py con Python 3.10 o posterior.



## Implementación

Fuerza bruta: El algoritmo empleado acá consiste en buscar y sumar de forma recursiva la mayor satisfacción en-
contrada para cada anime, comparando tiempos, energía y satisfacción acumulada, respetando las res-
tricciones dadas por el enunciado del laboratorio, probando todas las posibilidades hasta encontrar la
máxima satisfacción posible en un caso de prueba en cuestión.

Heurística de greedy 1:
Consiste en un enfoque de corto plazo, enfocados
solo en el capítulo siguiente y respetando las restricción mencionadas por el enunciado, realizando mul-
tiplicaciones y divisiones en un solo ciclo, aumentando la rentabilidad de cada capítulo, y en base a ello
maximizar la satisfacción obtenida.

Heurística de greedy 2: 
Esta implementacion busca planificar viendo cuánto costaría ver los animes completos y cuánta sa-
tisfacción total darían, obteniendo un ratio global. Luego realiza la ejecución real viendo los capítulos 1
por 1. Si se acaba el tiempo o la energía se abandona el anime y pasa al siguiente. Con todo calculado se
obtiene la satisfacción total maximizada al final de todo.

Programación dinámica:
Hace uso de una matriz bidimensional la cual
guarda la máxima satisfacción posible que se pueda alcanzar con el m tiempo y e energía disponible. Cal-
cula las opciones de visualización en paquetes respetando las restricciones, viéndolos como opciones
cerradas, escogiendo el más conveniente. Al escoger una combinación tras recorrer todos los paquetes
posibles, escoge la mejor opción posible con los recursos de tiempo y energía disponibles. guardan el
valor máximo.

general.h: Se ha decidio implementar el uso de un header para facilitar la ejecucion de cada algoritmo estudiado e implementado desde el programa principal.

### Programa principal

general.cpp es el programa principal.

Funciones:

writeResultToFile: Función que se encarga finalmente de escribir el resultado en el archivo de salida

get_MemoryUsage: Se encarga de medir el uso de la memoria RAM durante la ejecución de los archivos

Leer caso: Lee los casos de entrada considerando la cantidad de animes, capitulos, binificaciones, energia, etc. con tal de que se pueda realizar el calculo de la satisfaccion posteriormente.

ealizar_mediciones: Realiza las mediciones respectivas de tiempo y memoria llamando a las funciones respectivas, además de mandar a ejecutar los algoritmos en cuestion, además de escribir el resultado de las mediciones en un archivo de texto llamado "resultados_generales.txt" dentro de la carpeta "measurements"

main: Función principal, encargado de recibir los casos de entrada, verificarlas, y posteriormente mandar a ejecutar los algoritmos respectivos.

### Scripts

testcases_generator.py: Programa encargado de crear los archivos de prueba que quedan guardados en input.

plot_generator.py: Script encargado de tomar los datos escritos en "resultados_generales.txt" y genera los gráficos que representan los resultados generales.
