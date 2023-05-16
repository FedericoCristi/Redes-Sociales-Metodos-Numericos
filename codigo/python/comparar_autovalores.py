from ast import arg
from operator import truediv
from random import random

import sys
from time import process_time_ns
import numpy as np

import metodo_potencia
import deflacion

arg_path_1 = ""
arg_path_2 = ""

arg_iteraciones = 0
arg_tolerancia = 0
N = 0

#funcion que lee entradas
def parse():
    
    autov_python = np.genfromtxt(sys.argv[1], delimiter='\n')
    autov = np.genfromtxt(sys.argv[2], delimiter='\n')

    return autov_python, autov

autov_py, autov = parse()

diferencia = autov_py - autov
diferencia_abs = [ abs(x) for x in diferencia ] 

print(diferencia_abs)
