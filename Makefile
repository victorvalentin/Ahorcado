pruebas: pruebas.o funciones.o
	gcc pruebas.o funciones.o -o pruebas

pruebas.o: pruebas.cpp headers.h
	gcc -c pruebas.cpp -o pruebas.o

funciones.o: funciones.cpp headers.h
	gcc -c funciones.cpp -o funciones.o


servidor: servidor.o
	gcc servidor.o -o servidor
servidor.o: conectando.cpp headers.h
	gcc -c conectando.cpp -o servidor.o


cliente: cliente.o
	gcc cliente.o -o cliente

cliente.o: cliente.cpp headers.h
	gcc -c cliente.cpp -o cliente.o


clean:
	rm -f ./*.o
	rm pruebas
