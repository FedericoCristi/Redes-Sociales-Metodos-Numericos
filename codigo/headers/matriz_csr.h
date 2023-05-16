#ifndef __MATRIZ_CSR__
#define __MATRIZ_CSR__

#include <stdio.h>
#include <vector>
#include <math.h>
#include "matriz_coo.h"
#include "definiciones.h"

class matriz_csr
{
private:
    int _filas;
    int _cols;

public:

    int _NZ;
    double *_A = nullptr; //array de doubles, tamaño nz
    int *_JA = nullptr;   //array de ints, tamaño nz
    int *_IA = nullptr;   //array de ints, tamaño filas+1 

    matriz_csr(int n); //te da la identidad de dimension nxn
    matriz_csr(int filas, int cols); //nada
    matriz_csr(int filas, int cols, int nz, double *a, int *ja, int *ia); //matriz ya inicializada
    ~matriz_csr();

    int cant_filas();
    int cant_columnas();
    
    bool es_zero();

    int calcularGrado(int pagina_j); //cantidad de links salientes de j

    double get(int fila, int col);
    //void set(int fila, int col, double val);
    //el problema de csr es que se arma para un nz en especifico.
    //agregar elementos implica agrandar los arrays y modificarlos segun que elemento agregamos.

    matriz_csr mult_por_escalar(double escalar);
    matriz_csr multiplicar_por(matriz_csr* b);
    matriz_csr restar(matriz_csr* b);

    matriz_csr mult_d(std::vector<double> d);
    std::vector<double> mult_por_vector(std::vector<double> d);

    matriz_csr construir_matriz_laplace();
    std::vector<double> centralidad_autovector(std::vector<double> v, double autovalor);

    matriz_vectores_t csr_a_matriz_vectores();
    double dist_al_origen();

    void imprimir_debug();

};

#endif
