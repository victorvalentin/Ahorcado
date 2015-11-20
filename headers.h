#ifndef HEADERS_H
#define HEADERS_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<math.h>
#include<stdio_ext.h>
#include<time.h>
#include <signal.h>

/*Prototipos de funciones*/
int cantidad_lineas_archivo(FILE* fp);
int generar_num_linea(int cant_lineas);
char* leer_linea(FILE* fp, int num_linea);

#endif
