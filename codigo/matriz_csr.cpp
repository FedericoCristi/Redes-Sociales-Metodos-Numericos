#include "headers/matriz_csr.h"

using namespace std;

matriz_csr::matriz_csr(int n) : _filas(n), _cols(n){
    //identidad de nxn
    _A = new double[n];
    _JA = new int[n];
    _IA = new int[n+1];
    _NZ = n;

    for (int i = 0; i < n; i++){
        _A[i] = 1.0;
        _JA[i] = i;
        _IA[i] = i;
    }
    _IA[n] = n;
}

matriz_csr::matriz_csr(int filas, int cols) : _filas(filas), _cols(cols){
    //esto se usa?
}

matriz_csr::matriz_csr(int filas, int cols, int nz, double *a, int *ja, int *ia) : 
    _filas(filas), _cols(cols), _NZ(nz), _A(a), _JA(ja), _IA(ia){
}

matriz_csr::~matriz_csr()
{
    delete[] _A;
    delete[] _JA;
    delete[] _IA;
    _A = nullptr;
    _JA = nullptr;
    _IA = nullptr;
    
}

int matriz_csr::cant_filas(){
    return _filas;
}

int matriz_csr::cant_columnas(){
    return _cols;
}

//La matriz tiene todos sus elementos menores a epsilon.
bool matriz_csr::es_zero(){
    bool res = true;
    for (size_t i = 0; i < this->_NZ; i++){
        
        if(_A[i] > EPSILON){
            res = (res && false);
        }

    }
    return res;
}

//cantidad de links salientes de j
//la matriz actual supongo que es W
int matriz_csr::calcularGrado(int pagina_j){
    int count = 0;
    for(int fila = 1; fila <= _filas; fila++){
        if( this->get(fila, pagina_j) == 1 ){
            count++;
        }
    }
    return count;
} 

matriz_csr matriz_csr::mult_d(vector<double> d){

    //al ser una resta, matriz a y b tienen las mismas dimensiones. 
    int filas = this->cant_filas();
    int cols =  this->cant_columnas();
    
    //Inicializo el csr res con vectores para manipular más facil con los valores de A
    
    int res_NZ = this->_NZ;

    vector<int> res_JA ((res_NZ), 0);
    vector<int> res_IA ((filas+1), 0);
    vector<double> res_A ((res_NZ), 0);

    for(int j = 0; j < res_A.size(); j++){
        res_A[j] = this->_A[j];
        res_JA[j] = this->_JA[j];
    }

    for(int j = 0; j < res_IA.size(); j++){
        res_IA[j] = this->_IA[j];
    }

    for (int i = 0; i < this->cant_filas(); i++){

        int p_a = res_IA[i];
        int f_a = res_IA[i+1];

            while(p_a < f_a){
                //if(d[res_JA[p_a]] == 0){
                if(d[res_JA[p_a]] == 0 || d[res_JA[p_a]] < EPSILON){
                    res_NZ--;
                    res_A.erase(res_A.begin()+ p_a);
                    res_JA.erase(res_JA.begin()+ p_a);
                    f_a--;
                    for(int k = i+1; k < res_IA.size(); k++){
                        res_IA[k]--;
                    }
                }
                else{
                    res_A[p_a] = res_A[p_a] * d[res_JA[p_a]];
                    p_a++;
                }
            }
    }
    
    //paso los vectores a array
    double * res_A_arr = new double [res_A.size()];
    for(int i = 0; i < res_A.size(); i++){
        res_A_arr[i] = res_A[i];
    }

    int * res_JA_arr = new int [res_JA.size()];
    for(int i = 0; i < res_JA.size(); i++){
        res_JA_arr[i] = res_JA[i];
    }

    int * res_IA_arr = new int [res_IA.size()];
    for(int i = 0; i < res_IA.size(); i++){
        res_IA_arr[i] = res_IA[i];
    }

    matriz_csr* res = new matriz_csr(filas, cols, res_NZ, res_A_arr, res_JA_arr, res_IA_arr);

    return *res;
}

vector<double> matriz_csr::mult_por_vector(vector<double> d){

    int filas = this->cant_filas();
    int cols =  this->cant_columnas();

    //Inicializo el csr res con vectores para manipular más facil con los valores de A
    int res_NZ = this->_NZ;
    vector<int> res_JA ((res_NZ), 0);
    vector<int> res_IA ((filas+1), 0);
    vector<double> res_A ((res_NZ), 0);

    //recuerdo: A tiene elems
    //JA columnas de cada elem en A
    //IA contiene indices i,j entre los que se encuentra la fila i

    for(int j = 0; j < res_A.size(); j++){
        res_A[j] = this->_A[j];
        res_JA[j] = this->_JA[j];
    }

    for(int j = 0; j < res_IA.size(); j++){
        res_IA[j] = this->_IA[j];
    }

    vector<double> v(this->cant_filas(),0);

    //para c elemento del vector
    for (int i = 0; i < this->cant_filas(); i++){

        int p_a = res_IA[i]; //principio fila i de A
        int f_a = res_IA[i+1]; //fin fila i de  A
        double acc = 0;

        while(p_a < f_a){
            double col = d[res_JA[p_a]]; //elemento del vector
            acc += (res_A[p_a] * col);
            //printf("mult A[%d][%d] = %f  por v[%d] = %f \n", p_a, res_JA[p_a], res_A[p_a], res_JA[p_a], col);
            p_a++;
        }

        v[i] = acc;
    }

    //RETURN VECTOR
    return v;
}


matriz_csr matriz_csr::construir_matriz_laplace(){
    
    int n = this->cant_filas();
    
    matriz_csr diagonal(n);
    for(int i = 0; i < n; i++){
        int p_a = this->_IA[i];
        int f_a = this->_IA[i+1];
        double d = f_a - p_a;
        diagonal._A[i] = d;
    }

    matriz_csr laplace = diagonal.restar(this);
    
    double* a = new double[laplace._NZ];
    int* ja = new int[laplace._NZ];
    int* ia = new int[n+1];

    for (size_t i = 0; i < laplace._NZ; i++){
        a[i] = laplace._A[i];
        ja[i] = laplace._JA[i];
    }
    
    for (size_t i = 0; i < n+1; i++){
        ia[i] = laplace._IA[i];
    }

    matriz_csr* res = new matriz_csr(laplace.cant_filas(), laplace.cant_columnas(), laplace._NZ, a, ja, ia);
    return *res;
}

vector<double> matriz_csr::centralidad_autovector(vector<double> v, double autovalor){

    vector<double> x_res(v.size(),0);

    for (int i = 0; i < v.size(); i++){

        double acc = 0;
        int p_a = this->_IA[i];
        int f_a = this->_IA[i+1];

        while(p_a < f_a){
            double Aij = this->_A[p_a];
            double col = this->_JA[p_a];
            acc = v[col] * Aij;
            p_a++;
        }
        x_res[i] = acc/autovalor;
    }

    return x_res;

}

//void matriz_csr::set(int fila, int col, double val){
//agregar elementos a una csr implicaria cambiar la estructura actual, en algunos casos, agrandar los arrays que la conforman
//}


matriz_csr matriz_csr::mult_por_escalar(double escalar){
    int filas = this->cant_filas();
    int cols = this->cant_columnas();
    int elems_nz = this->_NZ;

    double *nuevoA = new double[elems_nz];
    int *nuevoJA = new int[elems_nz];
    int *nuevoIA = new int[filas+1];

    for(int i = 0; i < elems_nz; i++){
        nuevoA[i] = this->_A[i]*escalar;
        nuevoJA[i] = this->_JA[i];
    }

    for(int i = 0; i < filas+1; i++){
        nuevoIA[i] = this->_IA[i];
    }
    
    return matriz_csr(filas, cols, elems_nz, nuevoA, nuevoJA, nuevoIA);
}

double matriz_csr::get(int fila, int col){

    if(fila < 1 || fila > this->cant_filas() || col < 1 || col > this->cant_columnas()){
        printf("Se intento acceder a un elem en (%d, %d), pero hay %d filas y %d columnas\n", fila, col, _filas, _cols);
        return -1;
    } else {
        int start_a = _IA[fila-1];
        int end_a = _IA[fila];
        double elem = 0;

        int i;
        for (i = start_a; i < end_a; i++){
            if(_JA[i] == col - 1){
                elem = _A[i];
                break;
            }
        }

        return elem;
    }

}

matriz_csr matriz_csr::multiplicar_por(matriz_csr *b){
    
    int filas_a = this->cant_filas();
    int cols_b = b->cant_columnas();
    matriz_coo res(filas_a, cols_b);

    int cols_a = this->cant_columnas();
    int filas_b = b->cant_filas();

    for(int filaRes = 0; filaRes < filas_a; filaRes++){
        for(int colRes = 0; colRes < cols_b; colRes++){
            
            double suma = 0;

            //si se pueden multiplicar, col A = fila B;
            for(int k = 0; k < cols_a; k++){
                suma += this->get(filaRes+1, k+1) * b->get(k+1, colRes+1);
            }

            res.set(filaRes+1,colRes+1, suma);
        }
    }

    matriz_csr out = res.convert_coo_to_csr();
    
    return out;
}
      

void matriz_csr::imprimir_debug(){
    printf("NZ = %d \n", _NZ);
    
    printf("A = [ ");
    for (int i = 0; i < _NZ; i++){
        printf("%f", _A[i]);
        if(i != _NZ-1){
            printf(", ");
        }
    }
    printf(" ]\n");

    printf("JA = [ ");
    for (int i = 0; i < _NZ; i++){
        printf("%d", _JA[i]);
        if(i != _NZ-1){
            printf(", ");
        }
    }
    printf(" ]\n");
    
    printf("IA = [ ");
    for (int i = 0; i < _filas+1; i++){
        printf("%d", _IA[i]);
        if(i != _filas){
            printf(", ");
        }
    }
    printf(" ]\n");
    
}

int min(int a, int b){
    int res = b;
    if (a <= b){
        res = a;
    }
    return res;
}

int max(int a, int b){
    int res = a;
    if (a <= b){
        res = b;
    }
    return res;
}


matriz_csr matriz_csr::restar(matriz_csr* b){

    //al ser una resta, matriz a y b tienen las mismas dimensiones. 
    int filas = this->cant_filas();
    int cols =  this->cant_columnas();
    
    //Inicializo el csr res con vectores para manipular más facil con los valores de A
    int res_NZ = this->_NZ;

    vector<int> res_JA ((res_NZ), 0);
    vector<int> res_IA ((filas+1), 0);
    vector<double> res_A ((res_NZ), 0);

    for(int j = 0; j < res_A.size(); j++){
        res_A[j] = this->_A[j];
        res_JA[j] = this->_JA[j];
    }

    for(int j = 0; j < res_IA.size(); j++){
        res_IA[j] = this->_IA[j];
    }
    
    //para cada fila de la matriz resultante
    for (int i = 0; i < this->cant_filas(); i++){

        //subset relevante de a   [p_a  ....... f_a]
        int p_a = res_IA[i];
        int f_a = res_IA[i+1];

        int p_b = b->_IA[i];
        int f_b = b->_IA[i+1]; 

        //en el subset relevante de b
        while(p_b < f_b){

            int col_a;
            int col_b = b->_JA[p_b];

            //a no tiene elementos en la fila i
            if(p_a == f_a){
                //col_a = b->cant_filas();           //    !!!!Cambie esto<<<<<<<<<<<<<<<<<
                col_a = b->cant_columnas();
            } else {
                col_a = res_JA[p_a];               //col a es ja[p_a]
            }

            //si la colunna de b es menor a la primera columna del elem de A, solo queda agregar el -elem_b a A_res
            //---->   caemos en este caso si a no tiene elementos en la fila i    <------
            if(col_b < col_a){
                res_A.insert(res_A.begin() + (p_a), -b->_A[p_b]);
                res_JA.insert(res_JA.begin() + (p_a), col_b);

                //pasamos al prox elem de b
                p_b++;
                f_a++;   //??????
                res_NZ++;
                
                //la fila i pasa a tener 1 elemento mas. Desde i+1 deberia sumar 1 a todos los indices de rangos de IA
                
                //for(int k = i; k < res_JA.size(); k++){   !!!!Cambie esto<<<<<<<<<<<<<<<<<<<
                for(int k = i+1; k < res_IA.size(); k++){
                    res_IA[k]++;
                }

            //si las columnas coinciden, hay dos casos: 
            //          0 en la resta
            //          k en la resta
            } else if(col_b == col_a) {
                //caso donde obtengo 0 de la resta
                if (res_A[p_a] == b->_A[p_b]){
                    res_NZ--;
                    res_A.erase(res_A.begin()+ p_a);
                    res_JA.erase(res_JA.begin()+ p_a);

                    //pasamos a prox elem de b
                    p_b++;
                    f_a--; //?????

                    //la fila i pasa a tener 1 elemento menos. Desde i+1 deberia sumar 1 a todos los indices de rangos de IA
                    for(int k = i+1; k < res_IA.size(); k++){
                        res_IA[k]--;
                    }

                //caso donde obtengo k de la resta
                } else {

                    //ESTOY BASTANTE SEGURO DE QUE NO HACE FALTA ESTE IF YA
                    //no hay elementos en a
                    //no deberiamos llegar a este caso nunca
                    if(p_a == f_a){

                        printf("Entro a una linea en resta csr que no tenia que pasar nunca!\n");
                        res_A.insert(res_A.begin() + (p_a), -b->_A[p_b]);
                        res_JA.insert(res_JA.begin() + (p_a), col_b);
                        p_b++;
                        f_a++;
                        res_NZ++;

                        //tengo que empujar el indice de la fila (IA)
                        for(int k = i+1; k < res_IA.size(); k++){
                            res_IA[k]++;
                        }

                    } else {
                        res_A[p_a] -= b->_A[p_b];
                        p_a++;

                        //pasamos a prox elem de b
                        p_b++;
                        //no hace falta insertar columna
                    }
                }

            //si la col B > col A
            } else {

                //avanzo p_
                if(p_a != f_a){
                    p_a++;
                    col_a = this->_JA[p_a];
                }

                else{
                    res_A.insert(res_A.begin() + (p_a - 1), -b->_A[p_b]);
                    res_JA.insert(res_JA.begin() + (p_a - 1), col_b);
                    p_b++;
                    res_NZ++;
                    //tengo que empujar el indice de la fila (IA)
                    for(int k = i+1; k < res_IA.size(); k++){
                        res_IA[k]++;
                    }
                }
            }
        }


    }

    //paso los vectores a array
    double * res_A_arr = new double [res_A.size()];
    for(int i = 0; i < res_A.size(); i++){
        res_A_arr[i] = res_A[i];
    }

    int * res_JA_arr = new int [res_JA.size()];
    for(int i = 0; i < res_JA.size(); i++){
        res_JA_arr[i] = res_JA[i];
    }

    int * res_IA_arr = new int [res_IA.size()];
    for(int i = 0; i < res_IA.size(); i++){
        res_IA_arr[i] = res_IA[i];
    }

    matriz_csr* res = new matriz_csr(filas, cols, res_NZ, res_A_arr, res_JA_arr, res_IA_arr);

    /* POR SI QUERÉS DEBUGGEAR
    cout<<"filas: "<<res->cant_filas()<<endl;
    cout<<"columnas: "<<res->cant_columnas()<<endl; 
    for(int i = 0; i < res->_NZ; i++){
        cout<<"A: "<<res->_A[i]<<" de columna: "<< res->_JA[i]<<endl;
    }
    for(int i = 0; i < res->cant_filas()+1; i++){
        cout<<res->_IA[i]<<" "<<endl;
    }
    */
    return *res;
}

matriz_vectores_t matriz_csr::csr_a_matriz_vectores(){
    
    int filas = this->cant_filas();
    int cols = this->cant_columnas();

    matriz_vectores_t res;
    matriz_vectores_t::iterator it = res.begin();

    for (int fila = 0; fila < filas; fila++){
        //subsets de elementos
        int p_a = _IA[fila];
        int f_a = _IA[fila+1];

        vector<double> interior;
        //relleno todas las columnas con ceros
        for (int col = 0; col < cols; col++){
            interior.push_back(0);
        }
        res.push_back(interior);
        
        
        //la fila i no tiene elementos
        if(p_a == f_a){
            //no hago nada
        } else {
            for (int indiceElemA = p_a; indiceElemA < f_a; indiceElemA++){
                int col = _JA[indiceElemA];
                res[fila][col] = _A[indiceElemA];
            }
        }
    }
    
    return res;
}

double matriz_csr::dist_al_origen(){
    int cols = this->cant_columnas();
    double res = 0;

    for (size_t i = 0; i < cols; i++){
        double valor = this->get(i+1, 1);
        res += valor*valor;
    }

    return sqrt(res);
    
}


/*

Sea una matriz M ∈ Rm×n. Sea NZ a la cantidad de elementos no
nulos de M.
▶ Tenemos tres arreglos A, IA y JA.
▶ A y JA son de tama ̃no NZ .
▶ A guarda los valores de los elementos no nulos en orden:
primero por fila, luego columna (“row-major” order).
▶ JA guarda a qu ́e columna pertenece cada elemento de A.
▶ IA tiene m + 1 elementos. Guarda, para cada fila, los  ́ındices
en A entre los cuales est ́an sus elementos (inicio y fin).

*/
