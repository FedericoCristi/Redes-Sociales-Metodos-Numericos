#include "headers/auxiliares_centralidad.h"

matriz_vectores_t asignar_equipos(std::vector<double> autovectores[]){
    int N = autovectores->size();
    matriz_vectores_t res(N-1); //N-1 porque el ultimo autovalor no va a tener equipo asociado por ser 0

    //Armo los N-1 equipos
    for(int i = 0; i < N-1; i++){
        std::vector<double> equipo_i(N);

        //iteramos el autovalor i para separar
        for(int j = 0; j < N; j++){
            if(autovectores[i][j] < 0){
                equipo_i[j] = 1;
            } else {
                equipo_i[j] = 0;
            }
        }
        res[i] = equipo_i;
        printf("Equipos del autovector %d: \n", i);
        vector_imprimir(equipo_i);
    }

    return res;
}

//correlaciones con input de karate club
std::vector<double> calcular_correlaciones(std::vector<double> autovectores[], std::vector<double> distribucion){
    std::vector<double> res(distribucion.size());

    for(int i = 0; i < autovectores->size() - 1; i++){
        res[i] = abs(vector_correlacion(autovectores[i], distribucion));
        printf("Correlacion autovector %d - equipo: %f\n", i, res[i]);
        
    }

    return res;
}
