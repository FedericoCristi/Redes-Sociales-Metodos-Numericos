#ifndef __METODO_POTENCIA__
#define __METODO_POTENCIA__

#include "matriz_csr.h"
#include "auxiliares_vectores.h"

std::tuple<double, std::vector<double> > metodo_potencia(matriz_csr* A, std::vector<double> x0, int iteraciones, double errorMax);
matriz_csr* deflacion(matriz_csr *A, std::vector<double> v, double autovalor);
void metodo_potencia_y_deflacion(matriz_csr *matriz_entrada, double* array_autovalores, std::vector<double>* array_autovectores, int iteraciones, double tolerancia);

#endif