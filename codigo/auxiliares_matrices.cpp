#include "headers/auxiliares_matrices.h"

//Resta columnas por su valor medio
matriz_vectores_t procesar_datos(matriz_vectores_t input){
    matriz_vectores_t res = input;

    for (size_t col = 0; col < input[0].size(); col++)
    {
        //conseguimos vec col
        std::vector<double> vec_col;
        for (size_t fila = 0; fila < input.size(); fila++){
            double elem = input[fila][col];
            vec_col.push_back(elem);
        }

        double valor_medio_i = vector_valor_medio(vec_col);

        for (size_t fila = 0; fila < input.size(); fila++){
            double elem = input[fila][col] - valor_medio_i;
            res[fila][col] = elem;
        }
    }
    
    return res;
}

matriz_vectores_t matriz_cov(matriz_vectores_t x){
    //sea "x" m x n 
    //armamos matriz transpuesta "xt" n x m
    matriz_vectores_t xt (x[0].size(), std::vector<double>(x.size(),0));
    for(int i = 0; i < x.size(); i++){
        for(int j = 0; j < x[0].size(); j++){
            xt[j][i] = x[i][j];
        }
    }
    
    //hacemos el producto de matrices "x_res" m x m
    matriz_vectores_t x_res (x.size(), std::vector<double>(x.size(),0));
    for(int i = 0; i < xt.size(); i++){          //i = 0,...,m
        for(int it = 0; it < x[0].size(); it++){  //it = 0,...,m  
            double acc = 0;
            for(int j = 0; j < xt[0].size(); j++){  //j = 0,...,n   
                acc += xt[i][j] * x[j][it];
            }
            x_res[i][it] = acc/(x.size()-1); 
        }
    }

    return x_res;
}

matriz_vectores_t construir_red(matriz_vectores_t m, double umbral){
    matriz_vectores_t m_res (m.size(), std::vector<double>(m.size(),0));

    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m.size(); j++){
            if(m[i][j] > umbral){
                m_res[i][j] = 1;
            }
        }
    }

    return m_res;
}

matriz_vectores_t matriz_similaridad(matriz_vectores_t x){
    //sea "x" m x n 
    //armamos matriz transpuesta "xt" n x m
    matriz_vectores_t xt (x[0].size(), std::vector<double>(x.size(),0));
    for(int i = 0; i < x.size(); i++){
        for(int j = 0; j < x[0].size(); j++){
            xt[j][i] = x[i][j];
        }
    }
    
    //hacemos el producto de matrices "x_res" m x m
    matriz_vectores_t x_res (x.size(), std::vector<double>(x.size(),0));
    for(int i = 0; i < x.size(); i++){          //i = 0,...,m
        for(int it = 0; it < xt[0].size(); it++){  //it = 0,...,m  
            double acc = 0;
            for(int j = 0; j < x[0].size(); j++){  //j = 0,...,n   
                acc += x[i][j] * xt[j][it];
            }
            x_res[i][it] = acc; 
        }
    }

    return x_res;

}

void matriz_vectores_imprimir(matriz_vectores_t mat){

    for (int fila = 0; fila < mat.size(); fila++){
        
        printf("[ ");
        for (int col = 0; col < mat[fila].size(); col++){
            printf("%f", mat[fila][col]);
            if(col != mat[fila].size()-1){
                printf(", ");
            }

        }
        printf(" ]\n");
    }

   /* printf("Vector: \n");
    printf("[");

    for(int i = 0; i < p.second.size(); i++){
        printf("%f", p.second[i]);
        printf(",");
    }*/
    
    return;
}

matriz_vectores_t matriz_podar(matriz_vectores_t m, int k){
    matriz_vectores_t res (m.size(), std::vector<double> (k,0));

    if(k >= m[0].size()){
        printf("No corte nada, porque k > cantidad de columnas\n");
        return m;
    } else {
        for (size_t fila = 0; fila < m.size(); fila++){
            for (size_t col = 0; col < k; col++){
                res[fila][col] = m[fila][col];
            }
        }
        return res;
    }
}