#ifndef __AUX_CENTRALIDAD__
#define __AUX_CENTRALIDAD__

#include "definiciones.h"
#include "auxiliares_vectores.h"

matriz_vectores_t asignar_equipos(std::vector<double> autovectores[]);
std::vector<double> calcular_correlaciones(std::vector<double> autovectores[], std::vector<double> distribucion);

#endif