#include "headers/deflacion.h"

matriz_csr* deflacion_prototipo(matriz_csr *A, std::vector<double> v, double autovalor){

    //Armo  autovector * v * vt   

    std::vector<int> not_cero_index;

    for(int i = 0; i < v.size(); i++){
        if(v[i] != 0){
            not_cero_index.push_back(i);
        }
    }

    std::vector<double> res_A;
    std::vector<double> res_JA;
    std::vector<double> res_IA (A->cant_filas()+1, 0);

    printf("Armado de lamda * v * vt\n");
    int NZ = 0;
    for (size_t fila = 0; fila < A->cant_filas(); fila++){
        for (size_t col = 0; col < A->cant_filas(); col++){

            //double elem = v[not_cero_index[fila]]*v[not_cero_index[col]];   
            double elem = v[fila]*v[col]*autovalor; 
            if(elem != 0){
                res_A.push_back(elem);
                res_JA.push_back(col);
                for(int i = fila+1; i < res_IA.size(); i++){
                    res_IA[i]++;
                }
                NZ++;
            }
        }
    }
    
    double * res_A_t = new double [res_A.size()];
    for(int i = 0; i < res_A.size(); i++){
        res_A_t[i] = res_A[i];
    }

    int * res_JA_t = new int [res_JA.size()];
    for(int i = 0; i < res_JA.size(); i++){
        res_JA_t[i] = res_JA[i];
    }

    int * res_IA_t = new int [res_IA.size()];
    for(int i = 0; i < res_IA.size(); i++){
        res_IA_t[i] = res_IA[i];
    }

    matriz_csr avvt(A->cant_filas(), A->cant_columnas(), NZ, res_A_t, res_JA_t, res_IA_t); 
    avvt.imprimir_debug();

    printf("Fin de armado\n");
    
    //printf("coo a csr\n");
    //matriz_csr vvt = temp.convert_coo_to_csr();
    //printf("mult escalar\n");
    //matriz_csr autovalor_por_vvt = vvt.mult_por_escalar(autovalor);
    printf("restar\n");
    //matriz_csr A_menos_avvt = A->restar(&autovalor_por_vvt);
    matriz_csr A_menos_avvt = A->restar(&avvt);
    A_menos_avvt.imprimir_debug();
    printf("pasando a arrays..\n");


    double * res_A_arr = new double [A_menos_avvt._NZ];
    for(int i = 0; i < A_menos_avvt._NZ; i++){
        res_A_arr[i] = A_menos_avvt._A[i];
    }

    int * res_JA_arr = new int [A_menos_avvt._NZ];
    for(int i = 0; i < A_menos_avvt._NZ; i++){
        res_JA_arr[i] = A_menos_avvt._JA[i];
    }

    int * res_IA_arr = new int [A_menos_avvt.cant_filas()+1];
    for(int i = 0; i < A_menos_avvt.cant_filas()+1; i++){
        res_IA_arr[i] = A_menos_avvt._IA[i];
    }

    matriz_csr* res = new matriz_csr(A_menos_avvt.cant_filas(), A_menos_avvt.cant_columnas(), A_menos_avvt._NZ, res_A_arr, res_JA_arr, res_IA_arr);
    return res;
}
