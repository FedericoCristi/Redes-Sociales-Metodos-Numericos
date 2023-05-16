#ifndef __MATRIZ_COO__
#define __MATRIZ_COO__

#include <map>
#include "definiciones.h"
#include "matriz_csr.h"

class matriz_csr;

class matriz_coo
{
public:
    matriz_coo(int filas, int cols); //matriz vacia
    matriz_coo(int dimension, int not_cero, std::map< int,std::map<int, double>> elems); //matriz con elementos conocidos
    ~matriz_coo();
    
    int cant_filas();
    int cant_columnas();
    int cant_elems_not_cero();
    std::map< int, std::map<int, double> > conseguir_implementacion(); //esto es un monton

    void set(int fila, int col, double val);
    double get(int fila, int col);

    int calcularGrado(int pagina_j); //cantidad de links salientes de j

    void imprimir();
    void imprimir_links();
    void imprimir_debug();

    matriz_csr convert_coo_to_csr();
    
private:
    int _filas = 0;
    int _cols = 0;
    int _cant_elems_not_cero = 0;
    std::map< int, std::map<int, double> > _elems;
};

#endif