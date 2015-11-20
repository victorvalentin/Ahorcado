#include "headers.h"

int cantidad_lineas_archivo(FILE* fp)
{
	char cadena_aux[20]; // Para leer las lineas del archivo
	int cant_lineas = 0;

	fseek(fp, 0, SEEK_SET);				// Me posiciono al inicio
	fscanf(fp, "%s\n", cadena_aux);			// Leo un string y el \n

	while(!feof(fp))
	{
		cant_lineas++;
		fscanf(fp, "%s\n", cadena_aux);
	}

	return cant_lineas;
}

/* Va a devolver un numero entre 0 y cant_lineas*/
int generar_num_linea(int cant_lineas)
{
	time_t t;

	/* Intializes random number generator */
	srand((unsigned) time(&t));

	return (rand() % cant_lineas);
}

char* leer_linea(FILE* fp, int num_linea)
{
	int i;
	char* linea_leida = (char*) malloc(30*sizeof(char));	// Esto es memoria dinámica, hay que liberar en el main

	fseek(fp, 0, SEEK_SET);				// Me posiciono al principio por las dudas

	for(i=0; i<= num_linea; i++)
	{
		fscanf(fp, "%s\n", linea_leida);
	}

	return linea_leida;
}
