#include "headers/metodopotencia_eigen.h"

std::pair<double, Vector> metodoPotencia(const Matrix& A, Vector x0, int iteraciones, double error_tolerable)
{
  //  Vector x = Vector::Random(A.cols());
    Vector x = x0;
    Vector x_old = Vector::Zero(A.cols());
    double error = 0;
    double autovalor = 0;


    if(A.isZero(error_tolerable)){
        printf("En la iteracion se introdujo A nula, es unico autovalor posible es 0. Se devolvio el autovector lleno de unos.\n");
        Vector vec = Vector::Ones(A.cols());
        std::pair<double, Vector> res = std::make_pair(autovalor, vec);
        return res;
    }

    int iteracion = 0;
    while(iteracion < iteraciones){

    x = A*x ;
    x.normalize();
    double prod_escalar = x.dot(x_old);
    //double prod_escalar = vector_dot_prod(x, x_old);
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

    
        x_old = x;
        x = A * x;
        x.normalize();
    }
    autovalor = x.transpose().dot(A * x) / x.transpose().dot(x);



    return std::make_pair(autovalor, x);
}


std::pair<Vector, Matrix> calcularAutovectoresyValores(const Matrix& X, int num, int iteraciones, double error_tolerable)
{
    Matrix A(X);
    Vector autovalores(num);
    Matrix autovectores(A.rows(), num);
    Vector x0 = Vector::Random(num);

    for(int i=0;i<num;i++){
        std::pair<double,Vector> parAutovectoresyValores = metodoPotencia(A, x0, iteraciones,error_tolerable);
        autovalores(i) = parAutovectoresyValores.first;
        autovectores.col(i) = parAutovectoresyValores.second;
        A = A - parAutovectoresyValores.first * parAutovectoresyValores.second * parAutovectoresyValores.second.transpose();

    }
    return std::make_pair(autovalores, autovectores);
}

