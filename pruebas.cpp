#include "headers.h"

int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("Ingrese como parametro el archivo a leer\n");
		return -1;
	}

	int num_lineas = 0;
	FILE* fp = fopen(argv[1], "r");

	char* linea_leida;
	int num_linea_leer = 0;

	num_lineas = cantidad_lineas_archivo(fp);
	num_linea_leer = generar_num_linea(num_lineas);
	linea_leida = leer_linea(fp, num_linea_leer);

	printf("Cantidad de lineas del archivo: %d\n", num_lineas);
	printf("Numero generado aleatoriamente: %d\n", num_linea_leer);
	printf("Linea leida: %s\n", linea_leida);

	free(linea_leida);

	return 0;
}
