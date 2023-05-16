#include "headers/matriz_coo.h"


matriz_coo::matriz_coo(int filas, int cols) : _filas(filas), _cols(cols)
{
    _cant_elems_not_cero = 0;
}

matriz_coo::matriz_coo(int dimension, int not_cero, std::map< int,std::map<int, double> > elems) 
    : _filas(dimension), _cols(dimension), _cant_elems_not_cero(not_cero), _elems(elems)
{
}

matriz_coo::~matriz_coo()
{
    //borrar dicc elems?
}

int matriz_coo::cant_filas(){
    return _filas;
}

int matriz_coo::cant_columnas(){
    return _cols;
}

int matriz_coo::calcularGrado(int pagina_j){
    int count = 0;
    for(int fila = 1; fila <= _filas; fila++){
        if( this->get(fila, pagina_j) == 1 ){
            count++;
        }
    }
    return count;
} 

int matriz_coo::cant_elems_not_cero(){
    return _cant_elems_not_cero;
}

std::map< int, std::map<int, double> > matriz_coo::conseguir_implementacion(){
    return _elems;
}

double matriz_coo::get(int fila, int col){
    double res = _elems[fila][col];
    return res;
}

void matriz_coo::set(int fila, int col, double val){
    if(fila < 1 || fila > _filas || col < 1 || col > _cols){
        printf("Se intento agregar elem en (%d, %d), pero hay %d filas y %d columnas\n", fila, col, _filas, _cols);
    } else {

        //if(val == 0){
        if(abs(val) < EPSILON){
            
            //solo tiene sentido "agregar un 0" a una matriz coo si queremos borrar uno de sus elementos.
            if(_elems[fila][col] != 0){
                _cant_elems_not_cero--;
                _elems[fila].erase(col);
            }

        } else {

            if(_elems[fila][col] == 0){
                _cant_elems_not_cero++;
            }
            _elems[fila][col] = val;

        }
    }

}

void matriz_coo::imprimir(){
    for (std::map<int,std::map<int,double>>::iterator it=_elems.begin(); it!=_elems.end(); ++it){
        printf("[ ");
        for(std::map<int,double>::iterator itc=it->second.begin(); itc!=it->second.end(); ++itc){
            printf("%lf", itc->second);
            if(itc != it->second.end()--){
                    printf(", ");
            }
        }
        printf("]\n");
    }
}

void matriz_coo::imprimir_debug(){

    for (int fila = 1; fila <= _filas; fila++){
        printf("[ ");
        if(_elems.find(fila) == _elems.end()){
            //hay solo ceros en la fila
            for(int col = 1; col <= _cols; col++){
                printf("0");
                if(col != _cols){
                    printf(", ");
                }
            }
        } else {
            std::map<int, double> interior = _elems.find(fila)->second;
            for (int col = 1; col <= _cols; col++){

                if(interior.find(col) == interior.end()){
                    printf("0");
                } else {
                    printf("%d", (int)interior.find(col)->second);
                }

                if(col != _cols){
                    printf(", ");
                }

                
            }
        }
        printf(" ]\n");

    }
    printf("Cant elems not cero: %d\n", _cant_elems_not_cero);
    printf("Los ceros no estan en la estructura, son para visualizar la matriz\n\n");

}

void matriz_coo::imprimir_links(){
    for (std::map<int,std::map<int,double>>::iterator it=_elems.begin(); it!=_elems.end(); ++it){
        for(std::map<int,double>::iterator itc=it->second.begin(); itc!=it->second.end(); ++itc){
            printf("%d , ", it->first);
            printf("%d => ", itc->first);
            printf("%lf \n", itc->second);
        }
    }
}

matriz_csr matriz_coo::convert_coo_to_csr(){
    std::map< int, std::map<int, double>> imp_coo = this->_elems; 

    int filas = this->cant_filas();
    int cols = this->cant_columnas();
    int cant_nz = this->cant_elems_not_cero();

    double *A = new double[cant_nz];
    int *JA = new int[cant_nz];
    int *IA = new int[filas+1];

    int pos = 0;
    int posIA = 0;
    IA[posIA] = 0;

    for (int fila = 0; fila < filas; fila++){
        
        for (int col = 0; col < cols; col++) {
            double elem = this->get(fila+1, col+1);

            //if(elem != 0){
            if(elem != 0 && abs(elem) > EPSILON){
                A[pos] = elem;
                JA[pos] = col;
                pos++;
            }

        }
        posIA++;
        IA[posIA] = pos;
    }

    matriz_csr* res = new matriz_csr(filas, cols, cant_nz, A, JA, IA);
    
    return *res;
}