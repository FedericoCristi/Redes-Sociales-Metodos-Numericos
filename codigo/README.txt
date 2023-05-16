Instrucciones:

Compilacion: make

Ejecucion:

    El programa "tp1" recibe 3 parametros
        1. el path del archivo de la matriz <arg_nombre_archivo.txt>
            L> las comillas para el path son opcionales, unicamente requeridas si el nombre del archivo de entrada tiene espacios.
        2. un valor con el número de iteraciones                        <arg_numiter>
        3. un valor para el error tolerable para la convergencia del método de la potencia.  <arg_tolerancia>

    Una vez ejecutado, el programa crea archivos en la carpeta outputs llamados <arg_nombre_archivo_autovalores.txt>.out
    	<arg_nombre_archivo_autovectores.txt>.out
    	<pca_autovectores_eigen.txt>.out
    	<pca_autovalores_eigen.txt>.out
    	

    Su ejecucion es la especificada por el instructivo de entrega
    ./tp2    nombre_archivo  valor_num_int valor_tolerancia

    ejemplos de uso:
        ./tp2       karateclub_matriz.txt 5000 0.0001
            devuelve --->  outputs/karate_autovalores.out
            		    outputs/karate_autovectores.out
            		    outputs/pca_autovalores_eigen.out
            		    outputs/pca_autovectores_eigen.out

    Hay un valor global llamado TESTING_METODO_POTENCIA en main.cpp
    Sea este valor true, el programa termina antes de entrar a la parte de pca.
    Crea archivos en output/ donde se guarda "nombre_del_archivo_parametro" + tolerancia + autovalor / autovector segun corresponda.
            		    
            												

Breve descripcion de organizacion del programa:

    datos: archivos con los datos para el análisis de la red karate y la red ego Facebook.
    
    outputs: salidas con los archivos de los autovectores y autovalores de cada parte del tp. 

    python: programa en python de deflación y método de la potencia para comparar.
    
    pruebastp2: archivos txt y notebook utilizado para la elaboración del informe.
