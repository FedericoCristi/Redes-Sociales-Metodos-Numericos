from ast import arg
from operator import truediv
from random import random
from numpy import linalg as LA

import sys
from time import process_time_ns
import numpy as np

#PYTHON
#[[ 5.22720726e-01  3.68160356e-01  7.07106781e-01  3.02028137e-01]
# [ 5.22720726e-01  3.68160356e-01 -7.07106781e-01  3.02028137e-01]
# [ 2.81845199e-01 -8.15224745e-01  3.77427083e-16  5.05936655e-01]
# [ 5.22720726e-01  3.68160356e-01 -7.07106781e-01  3.02028137e-01]]


#[ 0.398332, 0.808713, 0.131701, 0.412273 ] 


vec_py = [ 2.81845199e-01, -8.15224745e-01,  3.77427083e-16,  5.05936655e-01]
vec_py = np.array(vec_py)

vec = [ 0.398332, 0.808713, 0.131701, 0.412273 ] 
vec = np.array(vec)

res = vec_py - vec

print(np.linalg.norm(res))