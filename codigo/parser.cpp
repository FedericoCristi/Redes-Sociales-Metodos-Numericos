#include "headers/parser.h"





matriz_coo* parseToCoo(char* path){    
    FILE* file = fopen(path, "r");
    matriz_coo* res;
    int N = 0;

    if(file == NULL){
        printf("El archivo %s no existe. \n", path);
        exit(1);
    } else {

        //cant de columnas, saco N y la matriz es cuadrada.
        while(fgetc(file) != '\n'){
            N++;
        }

        //TODO: esto esta re hardcordeado (asume que hay espacios entre los numeros) y que es cuadrada
        N = (N+1)/2;
        rewind(file);
        printf("N = %d\n", N);
        
        //llenar matriz con numeros
        res = new matriz_coo(N,N);
        int fila = 1;
        int col = 1;

        for (int i = 0; i < N*N; i++) {
            int elem;
            fscanf(file, "%d", &elem);

            fila = (i / N) + 1;
            col = (i % N) + 1;
            
            res->set(fila, col, elem);

        }

        fclose(file);
    }
    return res;
}

matriz_coo* parseToCooNoCuadrada(char* path){    
    FILE* file = fopen(path, "r");
    matriz_coo* res;
    int N = 0;
    int M = 0;

    if(file == NULL){
        printf("El archivo %s no existe. \n", path);
        exit(1);
    } else {

        //cant de filas, saco N
        int letter = fgetc(file);
        while(letter != EOF){
            
            if(letter == '\n'){
                N++;
            }

            letter = fgetc(file);
        }

        rewind(file);
        printf("Filas input = %d\n", N);

        

        //cant de columnas, saco M
        letter = fgetc(file);
        while(letter != '\n'){
            if(letter == ' '){
                M++;
            }

            letter = fgetc(file);
        }
        M++;
        rewind(file);
        printf("Columnas input = %d\n", M);

        
        //llenar matriz con numeros
        res = new matriz_coo(N,M);

        for (int f = 1; f <= N; f++){
            for (int c = 1; c <= M; c++){
                int elem;
                fscanf(file, "%d", &elem);                
                res->set(f, c, elem);
            }
        }

        fclose(file);
    }
    return res;
}

matriz_coo* parseToCooGrafo(char* path, std::map<int, int> dicc_user_id){
    //std::map<int, std::map<int,double>> res;
    //std::map<int,double> f1;
    
    FILE* file = fopen(path, "r");

    if(file == NULL){
        printf("El archivo %s no existe. \n", path);
        exit(1);
    } else {

        //por ser un grafo -> matriz de adyacncia, va a ser de nxn

        int N = 0;
        int letter = fgetc(file);
        while(letter != EOF){
            
            if(letter == '\n'){
                N++;
            }

            letter = fgetc(file);
        }

        rewind(file);
        printf("Filas input = %d\n", N);


        int cant_users = dicc_user_id.size();
        matriz_coo *res = new matriz_coo(cant_users,cant_users);
        
        for (int i = 0; i < N; i++) {
            int user1;
            fscanf(file, "%d", &user1);
            int user2;
            fscanf(file, "%d", &user2);

            res->set(dicc_user_id[user1]+1, dicc_user_id[user2]+1, 1);
        }

        
    return res;
    }
}


std::vector<double> parseToVec(char* path){
    std::vector<double> res;
    return res;
}

std::tuple<std::vector<double>, matriz_coo*> parseFacebookAtributos(char* path){
    matriz_coo *raw = parseToCooNoCuadrada(path);

    std::vector<double> users(raw->cant_filas(),0);
    for (int i = 0; i < raw->cant_filas(); i++){
        users[i] = raw->get(i+1, 1);
    }
    
    matriz_coo *atributos = new matriz_coo(raw->cant_filas(), raw->cant_columnas() - 1); //excluye numero de usuario
    for (int fila = 1; fila <= raw->cant_filas(); fila++){    
        for (int col = 2; col <= raw->cant_columnas(); col++){
            atributos->set(fila, col-1, raw->get(fila, col));
        }   
    }
    
    std::tuple<std::vector<double>, matriz_coo*> res = std::make_tuple(users, atributos);
    return res;
}



/*
archivo entrada: 
un archivo de texto con la matriz

archivo salida:
 debe entregar un archivo de texto con los autovalores y otro con los
autovectores como columnas de una matriz.

*/