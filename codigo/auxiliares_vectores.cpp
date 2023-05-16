#include "headers/auxiliares_vectores.h"

double vector_norma2(std::vector<double> &v){
    double res = 0;
    for(int i=0; i < v.size(); i++){
        res = res + (v[i]*v[i]);
    }
    res = sqrt(res);
    return res;
}

double vector_dot_prod(std::vector<double> &a, std::vector<double> &b){
    double res = 0;
    for(int i=0; i < a.size(); i++){
        res = res + (a[i]*b[i]);
    }
    return res;
}

void vector_normalizar(std::vector<double> &v){
    //printf("Normalizando: \n");
    double suma = 0;
    for(int i=0; i < v.size(); i++){
        suma = suma + (v[i]*v[i]);
        //printf("SUMA = %f\n", suma);
    }
    //printf("SUMA Total = %f\n", suma);
    suma = sqrt(suma);
    
    if(suma > EPSILON){
        for(int i=0; i < v.size(); i++){
            v[i] = v[i]/suma;
        }
    }

    return;
}

double vector_max_elem_abs(std::vector<double> &v){
    double max = 0;

    for(int i = 0; i < v.size(); i++){
        if(abs(v[i]) > max){
            max = abs(v[i]);
        }
    }

    return max;
}

void vector_imprimir(std::vector<double> &vec){
    printf("[ ");
    for (size_t i = 0; i < vec.size(); i++) {
        printf("%f", vec[i]);

        if (i != vec.size()-1) {
            printf(", ");
        } else {
            printf(" ] \n");
        }   
    }
    printf("\n");
        
}

std::vector<double> vector_por_escalar(std::vector<double> &v, double a){
    std::vector<double> res(v.size());

    for (size_t i = 0; i < v.size(); i++)
    {
        res[i] = v[i] * a;
    }

    return res;
}

std::vector<double> vector_restar_a_coordenadas(std::vector<double> &v, double a){
    std::vector<double> res(v.size());

    for (size_t i = 0; i < v.size(); i++)
    {
        res[i] = v[i] - a;
    }

    return res;
}

std::vector<double> vector_cuadrado(std::vector<double> &v){
    std::vector<double> res(v.size());

    for (size_t i = 0; i < v.size(); i++)
    {
        res[i] = v[i]*v[i];
    }
    
    return res;
}

double vector_valor_medio(std::vector<double> &v){
    double suma = 0;

    for (size_t i = 0; i < v.size(); i++){
        suma = suma + v[i];
    }

    suma = suma / v.size();

    return suma;
}

double vector_sumar_elem(std::vector<double> &v){
    double suma = 0;

    for (size_t i = 0; i < v.size(); i++){
        suma = suma + v[i];
    }

    return suma;
}



double vector_covarianza(std::vector<double> &a, std::vector<double> &b){
    double ux = vector_valor_medio(a);
    double uy = vector_valor_medio(b);
    int n = a.size();
    
    double cov = 0;

    std::vector<double> a_menos_ux = vector_restar_a_coordenadas(a, ux);
    std::vector<double> b_menos_uy = vector_restar_a_coordenadas(b, uy);

    cov = vector_dot_prod(a_menos_ux, b_menos_uy);

    cov = cov / (n - 1);

    return cov;
}

double vector_correlacion(std::vector<double> &a, std::vector<double> &b){
    double ux = vector_valor_medio(a);
    double uy = vector_valor_medio(b);
    int n = a.size();
    
    double cov = 0;
    double den = 0;

    std::vector<double> a_menos_ux = vector_restar_a_coordenadas(a, ux);
    std::vector<double> b_menos_uy = vector_restar_a_coordenadas(b, uy);

    cov = vector_dot_prod(a_menos_ux, b_menos_uy);

    double suma_a_menos_ux = vector_sumar_elem(a_menos_ux);
    double suma_b_menos_uy = vector_sumar_elem(b_menos_uy);

    a_menos_ux = vector_cuadrado(a_menos_ux);
    b_menos_uy = vector_cuadrado(b_menos_uy);

    double suma_a_menos_ux_cuadrado = vector_sumar_elem(a_menos_ux);
    double suma_b_menos_uy_cuadrado = vector_sumar_elem(b_menos_uy);



    double x_u_cuadrado_res = suma_a_menos_ux_cuadrado - (suma_a_menos_ux / n);
    double y_u_cuadrado_res = suma_b_menos_uy_cuadrado - (suma_b_menos_uy / n);

    den = sqrt(x_u_cuadrado_res * y_u_cuadrado_res);


    //b_menos_uy = vector_cuadrado(b_menos_uy);

    //den = sqrt(vector_dot_prod(a_menos_ux, b_menos_uy));

    cov = cov / den;

    return cov;
}
