#include "headers/metodos_potencia_deflacion.h"

std::tuple<double, std::vector<double> > metodo_potencia(matriz_csr* A, std::vector<double> x0, int iteraciones, double error_tolerable){   
    
    std::vector<double> x = x0;
    std::vector<double> x_old = x;
    double error = 0;
    double autovalor = 0;

    //printf("A: \n");
    //A->imprimir_debug();

    if(A->es_zero()){
        printf("En la iteracion se introdujo A nula, es unico autovalor posible es 0. Se devolvio el autovector lleno de unos.\n");
        std::vector<double> vec(x0.size(), 1);
        std::tuple<double, std::vector<double> > res = std::make_tuple(autovalor, vec);
        return res;
    }

    int iteracion = 0;
    while(iteracion < iteraciones){
        
        //printf("\n");
        //printf("Iteracion num %d: \n \n", iteracion);

        //printf("x: \n");
        //vector_imprimir(x);
        
        x = A->mult_por_vector(x);
        //printf("Ax: \n");
        //vector_imprimir(x);

        vector_normalizar(x); //Ax normalizado

        //printf("Ax (norm): \n");
        //vector_imprimir(x);

        //Recuerdo: < A, B > = |A| * |B| * cos(alpha)
        //Quiero que el vector normalizado, comparado con su anterior iteracion
        //tengan un angulo menor al error toletable.

        //cos(alpha) =  ( <A, B> ) / (|A| * |B|) < error tolerable

        //en particular, A y B estan normallizados. Tienen norma 1.
        //Ademas, como quiero que los vectores sean paralelos, cos(alpha) = 1

        //es decir, quiero <A, B> - 1 < error tolerable

        double prod_escalar = vector_dot_prod(x, x_old);
        //printf("prod escalar: %f \n", prod_escalar);

        error = abs(abs(prod_escalar) - 1);
        x_old = x;

        //printf("Error: %f \n", error);

        if(error < error_tolerable){    
            break;
        } else {
            iteracion++;
        }

    }
    
    //printf("Error: %f \n", error);

    if(iteracion == iteraciones){
        printf("El metodo de la potencia no convergio en el numero de iteraciones dado!.\n");
        exit(1);
    } else {

        //calculo de autovalor:
        //autovalor = (xt   *   Ax) / (xt * x)
        //abajo tengo la norma de x = 1

        std::vector<double> Ax = A->mult_por_vector(x);
        //double norma2v = vector_norma2(x);
        double xt_Ax = vector_dot_prod(Ax, x);
        autovalor = xt_Ax;// / norma2v;
        
        printf("Autovalor: %g \n", autovalor);
        printf("Autovector: ");
        vector_imprimir(x);
    }

    std::tuple<double, std::vector<double> > res = std::make_tuple(autovalor, x);
    return res;
}

matriz_csr* deflacion(matriz_csr *A, std::vector<double> v, double autovalor){

    //Armo  autovector * v * vt
    matriz_coo temp(A->cant_filas(), A->cant_columnas());

    for (size_t fila = 0; fila < temp.cant_filas(); fila++){
        for (size_t col = 0; col < temp.cant_columnas(); col++){
            
            double elem = v[fila]*v[col];
            temp.set(fila+1, col+1, elem);
        }
    }
    
    matriz_csr vvt = temp.convert_coo_to_csr();
    matriz_csr autovalor_por_vvt = vvt.mult_por_escalar(autovalor);
    matriz_csr A_menos_avvt = A->restar(&autovalor_por_vvt);

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

void metodo_potencia_y_deflacion(matriz_csr *matriz_entrada, double* array_autovalores, std::vector<double>* array_autovectores, int iteraciones, double tolerancia){
    
    //vector x0 inicial para el metodo de la potencia
    std::vector<double> x0(matriz_entrada->cant_filas());
    for (size_t i = 0; i < x0.size(); i++){
        x0[i] = (double)rand()/(double)RAND_MAX;
    }

    for (int i = 0; i < x0.size(); i++){
        //printf("CICLO %i: \n", i+1);
        //printf("Hace metodo de potencia: %i \n", i);
        std::tuple<double, std::vector<double>> res = metodo_potencia(matriz_entrada, x0, iteraciones, tolerancia);
        double autovalor = std::get<0>(res);
        std::vector<double> autovector = std::get<1>(res); 

        array_autovalores[i] = autovalor;
        array_autovectores[i] = autovector;

        matriz_csr* matriz_A_post_deflacion = deflacion(matriz_entrada, autovector, autovalor);
        *matriz_entrada = *matriz_A_post_deflacion;
        //matriz_A_post_deflacion->imprimir_debug();
    }


    //sort autovalores e autovectores segun abs 

    int index_max = 0;
    double temp = 0;
    std::vector<double> tempVec(x0.size());

    for (int i = 0; i < x0.size(); i++){
        index_max = i;

        for (int j = i; j < x0.size(); j++){
            if(abs(array_autovalores[j]) > array_autovalores[i]){
                index_max = j;
            }
        }

        if(index_max != i){
            temp = array_autovalores[i];
            tempVec = array_autovectores[i];
            
            array_autovalores[i] = array_autovalores[index_max];
            array_autovectores[i] = array_autovectores[index_max];
            
            array_autovalores[index_max] = temp;
            array_autovectores[index_max] = tempVec;
        }

    }
    


}