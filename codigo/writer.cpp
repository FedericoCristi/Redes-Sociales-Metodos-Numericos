#include "headers/writer.h"


/*
Como salida debe entregar un archivo de texto con los autovalores y otro con los
autovectores como columnas de una matriz.
*/

void writer_matriz_eigen(char* path, Matrix autovec, Vector autoval){
    std::string a = "output/";
    a.append(path);
    a.append("_(debug).out");

    FILE* output = fopen(a.c_str(), "w");
    

    std::string b = "output/";
    //b.append(path);
    b.append("pca_autovalores_eigen.out");

    FILE* output_autovalores = fopen(b.c_str(), "w");

    if(output_autovalores == NULL){
        printf("No se pudo crear output_autovalores en %s . \n", b.c_str());
        exit(1);
    } else {

        for (int i = 0; i < autoval.size(); i++){
            double autovalor = autoval(i);
            fprintf(output_autovalores, "%f\n", autovalor);
        }
        
        std::cout << "Autovalores escrita en: " << b << std::endl;
        fclose(output_autovalores);
    }

    std::string c = "output/";
    //c.append(path);
    c.append("pca_autovectores_eigen.out");

    FILE* output_autovectores = fopen(c.c_str(), "w");

    if(output_autovectores == NULL){
        printf("No se pudo crear output_autovectores en %s . \n", c.c_str());
        exit(1);
    } else {

        for (size_t elem = 0; elem < autovec.cols(); elem++){
            
            for (size_t vec = 0; vec < autovec.cols(); vec++){
                
                double v_elem = autovec(elem,vec);

                fprintf(output_autovectores, "%f ", v_elem);

                if(vec == autovec.cols()-1){
                    fprintf(output_autovectores, "\n");
                }



            }
            

        }
        
        
        std::cout << "Matriz de autovectores escrita en: " << c << std::endl;
        fclose(output_autovectores);
    }


}

void writer_debug_matriz(std::string path, matriz_coo* res, double* autovalores, std::vector<double>* autovectores, std::vector<double>* equipos){

    //ESCRIBIR MATRIZ
    std::string a = "output/";
    a.append(path);
    a.append("_(debug).out");

    FILE* output = fopen(a.c_str(), "w");

    if(output == NULL){
        printf("No se pudo crear output en %s . \n", a.c_str());
        exit(1);
    } else {

        for (int fila = 1; fila <= res->cant_filas(); fila++){
            for (int col = 1; col <= res->cant_columnas(); col++){
                
                int elem = (int)res->get(fila, col);

                fprintf(output, "%d ", elem);
                if(col == res->cant_columnas()){
                    fprintf(output, "\n");
                }

            }
        }
        
        std::cout << "Matriz (para chequear buena lectura) escrita en: " << a << std::endl;
        fclose(output);
    }

    //ESCRIBIR AUTOVALORES
    std::string b = "output/";
    b.append(path);
    b.append("_autovalores.out");

    FILE* output_autovalores = fopen(b.c_str(), "w");

    if(output_autovalores == NULL){
        printf("No se pudo crear output_autovalores en %s . \n", b.c_str());
        exit(1);
    } else {

        for (int i = 0; i < res->cant_filas(); i++){
            double autovalor = autovalores[i];
            fprintf(output_autovalores, "%f\n", autovalor);
        }
        
        std::cout << "Autovalores escrita en: " << b << std::endl;
        fclose(output_autovalores);
    }

    //ESCRIBIR EQUIPOS
    std::string e = "output/";
    e.append(path);
    e.append("_equipos.out");

    FILE* output_equipos = fopen(e.c_str(), "w");

    if(output_equipos == NULL){
        printf("No se pudo crear output_equipos en %s . \n", e.c_str());
        exit(1);
    } else {

        for (int i = 0; i < res->cant_filas(); i++){
            double eq = equipos->at(i);
            fprintf(output_equipos, "%d\n", (int)eq);
        }
        
        std::cout << "Equipos escritos en: " << e << std::endl;
        fclose(output_equipos);
    }
    
    //ESCRIBIR AUTOVECTORES COMO COLUMNAS DE UNA MATRIZ
    std::string c = "output/";
    c.append(path);
    c.append("_autovectores.out");

    FILE* output_autovectores = fopen(c.c_str(), "w");

    if(output_autovectores == NULL){
        printf("No se pudo crear output_autovectores en %s . \n", c.c_str());
        exit(1);
    } else {

        for (size_t elem = 0; elem < res->cant_columnas(); elem++){
            
            for (size_t vec = 0; vec < res->cant_columnas(); vec++){
                
                double v_elem = autovectores[vec][elem];

                fprintf(output_autovectores, "%f ", v_elem);

                if(vec == res->cant_columnas()-1){
                    fprintf(output, "\n");
                }

            }
            

        }
        
        
        std::cout << "Matriz de autovectores escrita en: " << c << std::endl;
        fclose(output_autovectores);
    }
}
