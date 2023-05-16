from ast import arg
from operator import truediv
from random import random

import sys
from time import process_time_ns
import numpy as np

import metodo_potencia
import deflacion

arg_path = ""
arg_iteraciones = 0
arg_tolerancia = 0
N = 0

#funcion que lee entradas
def parse():
    if len(sys.argv) != 4:
        print("Necesito argumentos:")
        print("1. input matriz")
        print("2. iteraciones")
        print("3. tolerancia")
        sys.exit()

    #inicializar globales
    global arg_path
    arg_path = sys.argv[1]
    global arg_iteraciones
    arg_iteraciones = int(sys.argv[2])
    global arg_tolerancia
    arg_tolerancia = float(sys.argv[3])

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
def write_output(res, autovalores, autovectores):

    #escribir matriz leida para saber si se leyo bien
    file = open("py_output_matriz(debug).txt", 'w')
    for fila in range(len(res)):
        for col in range(len(res[0])):
            file.write(str(int(res[fila][col])) + " ")
        file.write("\n")
    file.close()

    #escribir salida de autovalores
    file = open("py_output_autovalores.txt", 'w')
    for elem in autovalores:
        file.write(str(elem) + "\n")
    file.close()

    #escribir salida de autovectores
    file = open("py_output_autovectores.txt", 'w')
    for elem in range(len(autovectores[0])):
        for autovec in range(len(autovectores)):
            file.write(str(autovectores[autovec][elem]) + " ")
        file.write("\n")
    file.close()


W = parse()
print("W parseada: ")
print(W)
print("")

vector_aleatorio = np.zeros((N))
for i in range(N):
    vector_aleatorio[i] = 1

vector_autovalores = []
vector_autovectores = []

W_post_deflacion = W

for i in range(N):
    print("Ciclo numero: " + str(i))
    autovalor, autovector = metodo_potencia.metodo_potencia(W_post_deflacion, vector_aleatorio, arg_iteraciones, arg_tolerancia)

    print("autovalor = " + str(autovalor))
    print("autovector = ")
    print(autovector)
    print("")
    
    vector_autovalores.append(autovalor)
    vector_autovectores.append(autovector)

    W_post_deflacion = deflacion.deflacion(W_post_deflacion, autovector, autovalor)

write_output(W, vector_autovalores, vector_autovectores)
print("W impresa en py_output_matriz(debug).txt")
print("Autovalores impresos en py_output_autovalores.txt")
print("Autovectores impresos en py_output_autovectores.txt")

print("")
