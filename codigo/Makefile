OUT= tp2
INC= -I/headers -I/Eigen
OBJS= matriz_coo.cpp matriz_csr.cpp parser.cpp writer.cpp metodos_potencia_deflacion.cpp auxiliares_vectores.cpp auxiliares_matrices.cpp auxiliares_centralidad.cpp metodopotencia_eigen.cpp

main: main.cpp $(OBJS)
	g++ $(INC) $^ -o $(OUT) 

clean:
	rm -f *.o
	rm -f *.out
	rm -f $(OUT)