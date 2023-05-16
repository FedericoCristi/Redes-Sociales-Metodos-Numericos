#ifndef __AUXILIARES_VECTORES__
#define __AUXILIARES_VECTORES__

#include <stdio.h>
#include <vector>
#include <math.h>
#include "definiciones.h"

double vector_norma2(std::vector<double> &v);
double vector_dot_prod(std::vector<double> &a, std::vector<double> &b);
void vector_normalizar(std::vector<double> &v);

//otros
double vector_max_elem_abs(std::vector<double> &v);
void vector_imprimir(std::vector<double> &v);

//vectores y constantes
std::vector<double> vector_por_escalar(std::vector<double> &v, double a);
std::vector<double> vector_restar_a_coordenadas(std::vector<double> &v, double a);
std::vector<double> vector_cuadrado(std::vector<double> &v);

//medidas estadisticas de interes
double vector_valor_medio(std::vector<double> &v);
double vector_covarianza(std::vector<double> &a, std::vector<double> &b);
double vector_correlacion(std::vector<double> &a, std::vector<double> &b);

#endif