#ifndef __WRITER_H__
#define __WRITER_H__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "matriz_coo.h"
#include "metodopotencia_eigen.h"

//void writer(char* nombre_archivo, std::vector<double> res);
void writer_matriz_eigen(char* path, Matrix autovec, Vector autoval);

void writer_debug_matriz(std::string nombre_archivo, matriz_coo* matriz, double* autovalores, std::vector<double>* autovectores, std::vector<double>* equipos);


#endif