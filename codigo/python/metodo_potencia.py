# Power Method to Find Largest Eigen Value and Eigen Vector
# Importing NumPy Library
import numpy as np
import sys

def metodo_potencia(matriz, x0, iteraciones, max_error):
    # Reading order of matrix
    n = len(matriz)

    # Making numpy array of n x n size and initializing 
    # to zero for storing matrix
    a = matriz

    # Making numpy array n x 1 size and initializing to zero
    # for storing initial guess vector
    x = x0
    x_old = x
    
    # Reading tolerable error
    tolerable_error = max_error

    # Reading maximum number of steps
    max_iteration = iteraciones

    # Power Method Implementation
    lambda_old = 1.0
    condition =  True
    step = 1
    while condition:
        # Ax
        x = np.matmul(a,x)
        
        #normalizar Ax
        x = x / np.linalg.norm(x)
        
        # Checking maximum iteration
        if step > max_iteration:
            print('Not convergent in given maximum iteration!')
            exit()
        step = step + 1
        
        # Calculating error
        error = abs(abs(np.dot(x, x_old)) - 1)
        x_old = x
        #print('errror='+ str(error))
        
        condition = error > tolerable_error


    mx = np.dot(matriz,x)
    a = (np.dot(mx, x))
    #b = (np.dot(x, x)) es 1, x esta normalizado
    autovalor = a

    return autovalor, x