import numpy as np

def deflacion(matriz, autovector, autovalor):
    # Reading order of matrix
    n = len(matriz)

    # Making numpy array of n x n size and initializing 
    # to zero for storing matrix
    res = matriz

    # Making numpy array n x 1 size and initializing to zero
    # for storing initial guess vector
    v = autovector
    
    vvt = np.outer(v, v)

    lvvt = autovalor * vvt

    res = res-lvvt

    return res


    
    