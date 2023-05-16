from ast import arg
from operator import truediv

import sys
from time import process_time_ns
import numpy as np
from scipy import linalg

arg_path = ""
N = 0

#funcion que lee entradas
def parse():
    if len(sys.argv) != 2:
        print("Necesito argumentos:")
        print("1. input matriz")
        sys.exit()

    #inicializar globales
    global arg_path
    arg_path = sys.argv[1]

    #abrir archivo
    file = open(arg_path, 'r')
    line = file.readline()  #paginas
    global N
    N = 0

    #conseguir cantidad N de elementos en la matriz para saber la dimension
    for i in range(len(line) - 1):
        if line[i] != ' ' and line[i] != '\0' and line[i] != EOFError:
            N = N+1

    print("Dimensiones = (" + str(N) + ", " + str(N) + ")")
    
    #creo matriz res y vuelvo al principio del archivo
    dimensiones = (N,N)
    W = np.zeros(dimensiones)    
    file.seek(0)

    #cargo la matriz de entrada con los datos
    fila = 0
    col = 0
    lines = file.readlines()
    for line in lines:
        
        no_spaces_line = line.replace(" ", "")
        no_spaces_line = no_spaces_line.replace("\n", "")
        col = 0

        for char in no_spaces_line:
            if char != ' ' or char != '\n':
                W[fila][col] = int(char)
                col += 1

        fila += 1
    
    file.close()    
    return W

#funcion que escribe archivos
def write_output(autovalores, autovectores):

    #escribir salida de autovalores
    file = open("output/pwr_method/scipy_autovalores.txt", 'w')
    for elem in autovalores:
        file.write(str(elem) + "\n")
    file.close()

    #escribir salida de autovectores
    file = open("output/pwr_method/scipy_autovectores.txt", 'w')
    for elem in range(len(autovectores[0])):
        for autovec in range(len(autovectores)):
            file.write(str(autovectores[autovec][elem]) + " ")
        file.write("\n")
    file.close()

W = parse()
print("W parseada: ")
print(W)
print("")

x0 = np.zeros((N))
for i in range(N):
    x0[i] = 1

vector_autovalores = []
vector_autovectores = []

vector_autovalores, vector_autovectores = linalg.eig(W)

vector_autovalores = vector_autovalores.real
vector_autovectores = vector_autovectores.real

vector_autovalores_sort = []
vector_autovalores_sort = sorted(vector_autovalores, key=abs)
vector_autovalores_sort.reverse()

perm = [0]*len(vector_autovalores)

for i in range(0, len(vector_autovalores)):
    for j in range(0, len(vector_autovalores_sort)):    
        if vector_autovalores[i] == vector_autovalores_sort[j]:
            perm[i] = j
            break


vector_autovectores_sort = vector_autovectores
for i in range(0, len(vector_autovectores)):
    vector_autovectores_sort[i] = vector_autovectores[perm[i]]

print(vector_autovalores_sort)
print(vector_autovectores_sort)

write_output(vector_autovalores_sort, vector_autovectores_sort)
#print("W impresa en py_output_matriz(debug).txt")
#print("Autovalores impresos en py_output_autovalores.txt")
#print("Autovectores impresos en py_output_autovectores.txt")
#print("")
