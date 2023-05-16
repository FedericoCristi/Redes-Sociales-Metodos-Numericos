#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "headers/definiciones.h"
#include "headers/parser.h"
#include "headers/matriz_coo.h"
#include "headers/matriz_csr.h"
#include "headers/writer.h"

#include "headers/metodos_potencia_deflacion.h"
#include "headers/metodopotencia_eigen.h"

#include "headers/auxiliares_vectores.h"
#include "headers/auxiliares_matrices.h"
#include "headers/auxiliares_centralidad.h"

#define TESTING_METODO_POTENCIA false

int ITERACIONES;
double TOLERANCIA;
char* FILE_PATH;

Matrix podarKColumnas(const Matrix& A, int k);
Matrix convertiraMatrixEigen(matriz_vectores_t x);
std::vector<double> flatten(std::vector<std::vector<double> > m);
std::map<int, int> construir_dicc_usuarios_id(std::vector<double>);
matriz_coo* matriz_vectores_a_coo(matriz_vectores_t input);
matriz_vectores_t construir_red_eigen(Matrix m, int umbral);

void test();

void cargar_parametros(int argc, char** argv){
    char* param_iter;
    char* param_tolerancia;

    //Son 4 parametros, "./programa" "archivo matriz" "iteraciones" "tolerancia para convergencia"
    if(argc != 4){  
        printf("Quiero 3 parametros:\n1. path archivo matriz\n2. cant iteraciones\n3. tolerancia para convergencia\n");
        exit(1);
    } else {
        FILE_PATH = argv[1];
        param_iter = argv[2];
        param_tolerancia = argv[3];
    }

    //pasar iter y tolerancia de string a int
    ITERACIONES = atoi(param_iter);
    TOLERANCIA = atof(param_tolerancia);
}

typedef struct autovalores_autovectores{
    double *autovalores;
    std::vector<double> *autovectores;
    autovalores_autovectores(double* autoval, std::vector<double>* autovec) : autovalores(autoval), autovectores(autovec){}
    //TODO: borrar referencias a autovalores, autovectores
} autoval_autovec_t;

autoval_autovec_t obtener_autovalores_y_autovectores(matriz_csr& A){
    //array de autovalores (n doubles), array de autovectores (n vectores)
    matriz_csr entrada_csr = A;  //esta matriz es modificada en el metodo de la potencia.
    
    double *autovalores = new double[entrada_csr.cant_filas()];
    std::vector<double>* autovectores = new std::vector<double>[entrada_csr.cant_filas()];

    metodo_potencia_y_deflacion(&entrada_csr, autovalores, autovectores, ITERACIONES, TOLERANCIA);

    autoval_autovec_t res = autovalores_autovectores(autovalores, autovectores);

    return res;
}

int main(int argc, char** argv){

    //Inicializa file_path, iteraciones, tolerancia
    cargar_parametros(argc, argv);

    //parte 1 tp: metodo de la potencia, deflacion, centralidad, karateclub
    
    matriz_coo* matriz_entrada = parseToCoo(FILE_PATH);
    matriz_csr matriz_entrada_csr = matriz_entrada->convert_coo_to_csr();
    
    printf("Matriz entrada: \n");
    matriz_entrada->imprimir_debug();

    printf("============ Metodo de la potencia y deflacion    (1.1 y 1.2) ============\n\n");
    printf("Autovalores y autovectores de la matriz de adyacencia:\n\n");
    autoval_autovec_t info_matriz_entrada = obtener_autovalores_y_autovectores(matriz_entrada_csr);

    printf("\n============Calculos de centralidad de autovectores: (2.1) ============\n\n");
    std::vector<double> autovector_centrado = info_matriz_entrada.autovectores[0]; //autovector asociado al mayor autovalor (y estos vienen en orden mayor a menor)
    printf("Vector de centralidad sobre matriz de adyacencia:\n");
    vector_imprimir(autovector_centrado);

    //TODO: Analisis de los nodos mas centrales.

    //Centralidad sobre matriz de Laplace (2.2)
    printf("============Analisis de la matriz de Laplace: (2.2) ============\n\n");
    matriz_csr laplace = matriz_entrada_csr.construir_matriz_laplace();
    
    printf("Matriz de laplace:\n");
    laplace.imprimir_debug();

    //Metodo de la potencia y deflacion en matriz de laplace
    printf("\nAutovalores y autovectores de la matriz de laplace:\n\n");
    autoval_autovec_t info_matriz_laplace = obtener_autovalores_y_autovectores(laplace);

    std::vector<double> vector_centralidad = info_matriz_laplace.autovectores[0];
    printf("\nVector de centralidad sobre matriz de laplace:\n");
    vector_imprimir(vector_centralidad);
    
    //equipos
    printf("El ultimo autovalor es 0, por lo que lo ignoramos para los proximos calculos\n");
    printf("Separacion de equipos\n\n");
    matriz_vectores_t equipos = asignar_equipos(info_matriz_laplace.autovectores);

    //Input karateclub_labels
    std::vector<double> distribucion = {0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1};

    //correlaciones
    printf("Correlaciones:\n");
    std::vector<double> correlaciones(info_matriz_laplace.autovectores->size() - 1);
    correlaciones = calcular_correlaciones(info_matriz_laplace.autovectores, distribucion);
    printf("\n");
    
    //vector con MAYOR correlacion (mejor corte)
    int max_corr = 0;
    for(int i = 1; i < correlaciones.size(); i++){
        if(correlaciones[i] > correlaciones[max_corr]){
            max_corr = i;
        }
    }

    printf("Mejor autovector para corte, num %d:\n", max_corr);
    printf("Era autovector:\n");
    vector_imprimir(info_matriz_laplace.autovectores[max_corr]);
    printf("Equipo:\n");
    vector_imprimir(equipos[max_corr]);
    printf("Correlacion: %f\n\n", correlaciones[max_corr]);

    //escribir archivos

    if(TESTING_METODO_POTENCIA){
        std::string name = "test_";
        int n = matriz_entrada->cant_columnas();
        name.append(std::to_string(n));
        name.append("x");
        name.append(std::to_string(n));
        name.append("_t");
        name.append(std::to_string(TOLERANCIA));
        name.append("_");
        
        writer_debug_matriz(name, matriz_entrada, info_matriz_entrada.autovalores, info_matriz_entrada.autovectores, &(equipos[max_corr]));
        exit(0);
    } else {
        char name[7] = "karate";
        writer_debug_matriz(name, matriz_entrada, info_matriz_entrada.autovalores, info_matriz_entrada.autovectores, &(equipos[max_corr]));
    }

    /*
    Empieza parte 2 del tp, usamos un poco la biblioteca Eigen
    */

    //Analisis Ego-Facebook: Matriz de similiaridad y umbral u (3.1)
    printf("============Analisis Ego-Facebook: Matriz de similiaridad y umbral u (3.1) ============\n\n");
    
    //estos son los datos que analizamos
    //parseo del input feats a una tupla <vector de doubles, referencia a matriz coo>
    char matriz_atributos_path[24] = "datos/ego-facebook.feat";
    std::tuple<std::vector<double>, matriz_coo*> users_atributes = parseFacebookAtributos(matriz_atributos_path);
    std::vector<double> users = std::get<0>(users_atributes);
    matriz_coo *atributos = std::get<1>(users_atributes);

    //paso la coo a una matriz de vectores:
    matriz_csr atributos_csr = atributos->convert_coo_to_csr(); 
    matriz_vectores_t atributos_matriz_vectores = atributos_csr.csr_a_matriz_vectores();

    //calculo de matriz similaridad
    matriz_vectores_t atributos_matriz_similaridad = matriz_similaridad(atributos_matriz_vectores);

    //esta es la red real
    //parseo input edges a una matriz coo que representa un grafo (una matriz de adyacencia)
    char matriz_facebook_adyacencia_path[25] = "datos/ego-facebook.edges";
    std::map<int, int> dicc_user_id = construir_dicc_usuarios_id(users);
    matriz_coo *input_amistades = parseToCooGrafo(matriz_facebook_adyacencia_path, dicc_user_id);

    //paso la coo a una matriz de vectores:
    matriz_csr input_amistades_csr = input_amistades->convert_coo_to_csr();
    matriz_vectores_t input_amistades_matriz_vectores = input_amistades_csr.csr_a_matriz_vectores();

    //elegimos umbral arbitrario y construimos una red con eso
    double umbral = 5;
    matriz_vectores_t red = construir_red(atributos_matriz_similaridad, umbral);

    printf("============Analisis Ego-Facebook: Comparacion red amistades original y la matriz que construimos con los datos y umbral elegido (3.2) ============\n\n");

    std::vector<double> matriz_similar_flat = flatten(red);
    std::vector<double> red_ego_flat = flatten(input_amistades_matriz_vectores);

    double corr_flatt = vector_correlacion(red_ego_flat, matriz_similar_flat);
    printf("Umbral elegido %f\n", umbral);
    printf("Correlacion red original y nuestra matriz: %lf\n", corr_flatt);

    printf("============Analisis Ego-Facebook: Analisis de multiples umbrales, buscar el que mejor aproxima nuestros datos con la red original (3.3) ============\n\n");

    //Analisis de mejor umbral:
    //Comentado porque tarda bastante

    //for(double u = 0; u < 100; u++){
    //    matriz_vectores_t red = construir_red(atributos_matriz_similaridad, u);
    //    std::vector<double> matriz_similar_flat = flatten(red);
    //    std::vector<double> red_ego_flat = flatten(input_amistades_matriz_vectores);
    //    double corr_flatt = vector_correlacion(red_ego_flat, matriz_similar_flat);
    //    printf("Correlacion de red %lf = %lf\n", u, corr_flatt);
    //}
    
    
    printf("============Analisis Ego-Facebook: PCA (3.4) ============\n\n");

    //test();

    //armar matriz covarianza:
    //restarle a X la media de cada columna (a cada columnna)
    matriz_vectores_t x_proc = procesar_datos(atributos_matriz_vectores);
    Matrix matriz_datos_X = convertiraMatrixEigen(x_proc);
    
    Matrix xtx_cov = matriz_datos_X.transpose() * matriz_datos_X;
    Matrix xtx_covFinal = xtx_cov * (1/(xtx_cov.size()-1));
    Vector b = Vector::Ones(xtx_cov.cols());
    printf("\n %f",xtx_cov(0, 0));
    
    printf("\nAutovalores y autovectores de la matriz de covarianza:\n\n");


    std::pair<Vector, Matrix> parEigen = calcularAutovectoresyValores(xtx_cov, xtx_cov.cols(), 5000, 0.00001);
    //descomentar lo de abajo si quiero que imprima los resultados en archivo out
    //writer_matriz_eigen(FILE_PATH, parEigen.second, parEigen.first);


    printf("===============PCA=================\n");

    printf("%li\n ", matriz_datos_X.rows());

    Matrix matriz_V = parEigen.second;
    Matrix matriz_datos_Xt_base_cambiada = matriz_V.transpose() * matriz_datos_X.transpose();
    Matrix matriz_datos_X_base_cambiada = matriz_datos_Xt_base_cambiada.transpose();

    //Matrix datos_podada = podarKColumnas(matriz_datos_X_base_cambiada, 125);
    //printf("%i\n ", datos_podada.cols());
    /////////////////

    for (size_t k = 1; k < matriz_datos_X_base_cambiada.row(1).size(); k+=5)
    {
        printf("k = %li\n", k);
        //armar pca con k cortadas
        Matrix datos_podada = podarKColumnas(matriz_datos_X_base_cambiada, k);
        printf("Poda\n");
        //matriz_vectores_t matriz_similaridad_pca = matriz_similaridad(matriz_datos_podada);
        //hago la matriz similaridad
        Matrix datos_similaridad = datos_podada * datos_podada.transpose();
        printf("Similar\n");

        
        //size_t umbral = 7;
        //printf("Umbral = %i\n", umbral);
        //matriz_vectores_t red_pca = construir_red_eigen(datos_similaridad, umbral);
        
        
        for (int u = 0; u < 20; u++)
        {
            printf("u = %i, ", u);
            //comparar con red u
            //se arma la red para cada u respectivo y se compara con la red_ego dada
            matriz_vectores_t red_pca = construir_red_eigen(datos_similaridad, u);
            std::vector<double> matriz_similar_flat_pca = flatten(red_pca);
            //la "red_ego_flatten" ya la tenemos calculada desde antes
            double corr_flatt_pca = vector_correlacion(red_ego_flat, matriz_similar_flat_pca);
            printf("Correlacion de red = %lf\n", corr_flatt_pca);
        }
        
    }
    
    return 0;
}

matriz_vectores_t construir_red_eigen(Matrix m, int umbral){
    matriz_vectores_t m_res (m.row(0).size(), std::vector<double>(m.col(0).size(),0));

    for(int i = 0; i < m.row(0).size(); i++){
        for(int j = 0; j < m.col(0).size(); j++){
            if(m.row(i)[j] > umbral){
                m_res[i][j] = 1;
            }
        }
    }

    return m_res;
}

void test(){

    printf("============TESTS!! ============\n\n");


    matriz_coo matriz = matriz_coo(4,4);
    matriz.set(1,1,7);
    matriz.set(1,2,6);
    matriz.set(1,3,3);
    matriz.set(1,4,2);
    
    matriz.set(2,1,2);
    matriz.set(2,2,0);
    matriz.set(2,3,4);
    matriz.set(2,4,2);
    
    matriz.set(3,1,3);
    matriz.set(3,2,0);
    matriz.set(3,3,2);
    matriz.set(3,4,2);
    
    matriz.set(4,1,4);
    matriz.set(4,2,6);
    matriz.set(4,3,3);
    matriz.set(4,4,2);
    
    matriz_csr matriz_cambio_csr = matriz.convert_coo_to_csr();
    matriz_vectores_t matriz_vec = matriz_cambio_csr.csr_a_matriz_vectores();

    //printf("matriz pre procesar \n");
    //matriz_vectores_imprimir(matriz_vec);

    matriz_vec = procesar_datos(matriz_vec);
    matriz_vectores_t m_cov = matriz_cov(matriz_vec);

    //printf("matriz covarianza \n");
    //matriz_vectores_imprimir(m_cov);

    printf("============FIN TESTS!!============\n\n");
}


Matrix podarKColumnas(const Matrix& A, int k){
    Matrix res(A.rows(), k);
    for(int i = 0; i < k; i++){
        res.col(i) = A.col(i);
    }
    return res;
}

Matrix convertiraMatrixEigen(matriz_vectores_t x){
    int filas = x.size();
    int cols = x[0].size();

    Matrix res(filas, cols);

    for(int i = 0; i < filas; i++ ){
        for(int j = 0; j < cols; j++){
            res(i,j) = x[i][j];
            }
        }
        
    return res;
    
}

std::map<int, int> construir_dicc_usuarios_id(std::vector<double> users){
    std::map<int, int> res;

    for (size_t i = 0; i < users.size(); i++)
    {
        int user = (int)users[i];
        res[user] = i;
    }
    
    return res;
}



std::vector<double> flatten(std::vector<std::vector<double> > m){
    std::vector<double> res(m.size()*m.size(),0);
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j< m.size(); j++){
            res[i*m.size()+j] = m[i][j];
        }
    }
    return res;
}

matriz_coo* matriz_vectores_a_coo(matriz_vectores_t input){
    matriz_coo* res = new matriz_coo(input.size(), input[0].size());
    for (int fila = 0; fila < input.size(); fila++){
        for (int col = 0; col < input[0].size(); col++){
            res->set(fila+1, col+1, input[fila][col]);
        }   
    }
    return res;
}
