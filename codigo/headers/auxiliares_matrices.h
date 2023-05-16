#ifndef __AUX_MATRICES__
#define __AUX_MATRICES__

#include "definiciones.h"
#include "auxiliares_vectores.h"

matriz_vectores_t procesar_datos(matriz_vectores_t input);
matriz_vectores_t matriz_cov(matriz_vectores_t x);
matriz_vectores_t construir_red(matriz_vectores_t m, double umbral);
matriz_vectores_t matriz_similaridad(matriz_vectores_t m);
void matriz_vectores_imprimir(matriz_vectores_t mat);
matriz_vectores_t matriz_podar(matriz_vectores_t m, int k);

#endif