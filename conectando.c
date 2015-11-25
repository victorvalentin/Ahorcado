#include "headers.h"
#define PUERTO "6000"
#define MAX_JUG 8

struct dato{
  char nombre[15];
};

struct nodo{
	char letra;
	struct nodo *next;
};

int generar_num_linea(int);
int cantidad_lineas_archivo(FILE *);
void funcion_mensaje(int [], int,char *);
void leer_linea(FILE *, int, char *);
void generar_lista(struct nodo **, char[], int);
void comparar(struct nodo **, char);
int levantar_servidor(char* puerto, int listenSize);
void mostrar_jugadores(struct dato jugadores[] ,int cantidad);
void aceptar_jugadores(int, int, int [], struct sockaddr_in []);
void recibir_nombres(int [], int, struct dato[]);
void enviar_cantidad_letras(int [], int, int);
void abrir_archivo(FILE*, char*);
int generar_palabra(FILE*, char*);

int main(int argc,char *argv[])
{
	// Variables a usar
	int cant_jugadores;
	int fd_listener, fd_jugadores[MAX_JUG];
	int cant_letras;
	socklen_t addr_size = sizeof(struct sockaddr);
	int a=0, b=1, opcion, fin_de_juego, var3;
	char palabra[20], letra, palabra_completa[20];
	struct dato jugadores[MAX_JUG];
	struct sockaddr_in vector_cliente[MAX_JUG];
	struct nodo *inicio=NULL;
	FILE *fp;

	// Comprobando que los parametros esten bien
	if(argc<2)
	{
		printf("argc<2 Ingrese como argumento el archivo de palabras\n");
		exit(1);
	}

	fd_listener = levantar_servidor(PUERTO, MAX_JUG);

	printf("Ingrese la cantidad de jugadores\n");
	scanf("%d", &cant_jugadores);

	__fpurge(stdin);
	system("clear");

	aceptar_jugadores(fd_listener, cant_jugadores, fd_jugadores, vector_cliente);

	char* mensaje= malloc(20);
	strcpy(mensaje, "Ingrese un nombre");
	// Envio a los jugadores
	funcion_mensaje(fd_jugadores, cant_jugadores, mensaje);
	free(mensaje);

	recibir_nombres(fd_jugadores, cant_jugadores, jugadores);

	system("clear");

	mostrar_jugadores(jugadores ,cant_jugadores);

	abrir_archivo(fp, argv[1]);
	cant_letras = generar_palabra(fp, palabra);
	enviar_cantidad_letras(fd_jugadores, cant_jugadores, cant_letras);

	generar_lista(&inicio, palabra, cant_letras);

	var3 = 0;
	int var2;

	while(1)
	{
		if(inicio==NULL)
		{
			break;
		}

		if((send(fd_jugadores[var3], &a, sizeof(a), 0))==-1)
		{
			printf("\nerror al enviar a\n");
			exit(1);
		}

		else
		{
			if((recvfrom(fd_jugadores[var3], &opcion, sizeof(opcion), 0, (struct sockaddr*)&vector_cliente[var3], &addr_size))< 0)
			{
				perror("\nerror al recibir opcion: ");
				exit(1);
			}
			else
			{
				if(opcion==1)
				{
					// Recibe una letra
					if((recv(fd_jugadores[var3], &letra, sizeof(letra), 0))==-1)
					{
						printf("error al recibir letra del jugador %s\n", jugadores[var3].nombre);
						exit(1);
					}
					else
					{
						comparar(&inicio, letra);

						for(var2=0; var2 <= cant_jugadores-1; var2++)
						{
							if(var2==var3)
							{
								var2++;
							}

							if((send(fd_jugadores[var2],&b,sizeof(int),0))==-1)
							{
								printf("\nerror al enviar b\n");
								exit(1);
							}
							else
							{
								if((send(fd_jugadores[var2],jugadores[var3].nombre,15,0))==-1)
								{
									printf("error al enviar nombres\n");
									exit(1);
								}
								else
								{
									if((send(fd_jugadores[var2],&opcion,4,0))==-1)
									{
										printf("error al enviar la opcion a los demas jugadores\n");
										exit(1);
									}
									else
									{
										if((send(fd_jugadores[var2],&letra,1,0))==-1)
										{
											printf("error al enviar la letra a los demas jugadores\n");
											exit(1);
										}
										else
										{
											if(inicio==NULL)
											break;
										}
									}
								}
							}
						}
					}
				}
				else
				{
					if((recv(fd_jugadores[var3],palabra_completa,20,0))==-1)
					{
						printf("error al recibir palabra completa\n");
						exit(1);
					}
					else
					{
						for(var2=0;var2<=cant_jugadores-1;var2++)
						{
							if(var2==var3)
							{
								var2++;
							}
							if((send(fd_jugadores[var2],&b,sizeof(b),0))==-1)
							{
								printf("error al enviar b\n");
								exit(1);
							}
							else
							{
								if((send(fd_jugadores[var2],jugadores[var2].nombre,15,0))==-1)
								{
									printf("error al enviar nombres\n");
									exit(1);
								}
								else
								{
									if((send(fd_jugadores[var2],&opcion,4,0))==-1)
									{
										printf("error al enviar la opcion a los demas jugadores\n");
										exit(1);
									}
									else
									{
										if((send(fd_jugadores[var2],palabra_completa,20,0))==-1)
										{
											printf("error al enviar la palabra completa a los demas jugadores\n");
											exit(1);
										}
										else
										{
											if((strcmp(palabra,palabra_completa))==0)
											{
												break;
											}
											else
											{
												fin_de_juego++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if(var3==cant_jugadores)
		{
			var3=0;
		}
	}
	return 0;
}

void funcion_mensaje(int array_fd[], int cantidad, char *mensaje)
{
	int var;
	int long_msg = strlen(mensaje) + 1;

	for(var=0; var < cantidad; var++)
	{
		if((send(array_fd[var], mensaje, long_msg, 0))==-1)
		{
			perror("error al enviar el mensaje: ");
			exit(1);
		}
	}
}

int cantidad_lineas_archivo(FILE* fp)
{
	char cadena_aux[20]; 				// Para leer las lineas del archivo
	int cant_lineas = 0;

	fseek(fp, 0, SEEK_SET);				// Me posiciono al inicio
	fscanf(fp, "%s\n", cadena_aux);		// Leo un string y el \n

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
	/* Intializes random number generator */
	srand(time(NULL));
	return (rand() % cant_lineas);
}

void leer_linea(FILE* fp, int num_linea, char *palabra)
{
	int i;
	char* linea_leida = (char*) malloc(30*sizeof(char));	// Esto es memoria dinámica, hay que liberar

	fseek(fp, 0, SEEK_SET);				// Me posiciono al principio por las dudas

	for(i=0; i<= num_linea; i++)
	{
		fscanf(fp, "%s\n", linea_leida);
	}

	strcpy(palabra,linea_leida);
	free(linea_leida);
}

void generar_lista(struct nodo **ppio, char palabra[], int cant_letras)
{
	int k;

	for(k=0; k < cant_letras; k++)
	{
		struct nodo *a, *b;

		if(!(b = (struct nodo *)malloc(sizeof(struct nodo))))
		{
			perror("Error al pedir recursos: ");
			exit(1);
		}

		b->next = NULL;
		b->letra = palabra[k];

		if(*ppio == NULL)
		{
			*ppio = b;
		}
		else
		{
			for(a = *ppio; a->next != NULL; a = a->next);
			a->next = b;
		}
	}
}

void comparar(struct nodo **ppio,char T)
{
	struct nodo *a, *b;
	a = *ppio;

	if(a->letra==T)
	{
		*ppio = a->next;
		free(a);
	}

	for(a = *ppio; a!=NULL; a = a->next)
	{
		if(a != NULL)
		{
			if(a->letra == T)
			{
				b = *ppio;

				while(b->next != a && b->next != NULL)
				{
					b = b->next;
				}

				b->next = a->next;
				free(a);
			}
		}

		break;
	}
}

int levantar_servidor(char* puerto, int listenSize)
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(NULL, puerto, &hints, &res);

    // make a socket, bind it, and listen on it:
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd != -1)
    {
		bind(sockfd, res->ai_addr, res->ai_addrlen);
		listen(sockfd, listenSize);
    }

    return sockfd;
}

void mostrar_jugadores(struct dato jugadores[] ,int cantidad)
{
	int i;

	for(i=0; i < cantidad; i++)
	{
		printf("Jugador: %s conectado\n", jugadores[i].nombre);
	}
}

void aceptar_jugadores(int fd, int cantidad_jugadores, int vectorfd[], struct sockaddr_in clientes[])
{
	int x;

	for(x=0; x < cantidad_jugadores; x++)
	{
		socklen_t addr_len =sizeof(struct sockaddr);

		if((vectorfd[x] = accept(fd, (struct sockaddr *)&(clientes[x]), &addr_len))==-1)
		{
			printf("\nerror en accept\n");
			exit(1);
		}
		printf("Jugador numero %d conectado\n", x+1);
	}
}

void recibir_nombres(int vector_fd[], int cant_jugadores, struct dato jugadores[])
{
	int var2;

	for(var2=0; var2< cant_jugadores; var2++)
	{
		char* buffer_nombre = malloc(20);

		if((recv(vector_fd[var2], buffer_nombre, 20, 0))==-1)
		{
			perror("\nError al recibir nombres: ");
			exit(1);
		}

		strcpy(jugadores[var2].nombre, buffer_nombre);

		free(buffer_nombre);
	}
}

void enviar_cantidad_letras(int vectorfd[], int cant_jugadores, int cant_letras)
{
	int j;

	for(j=0; j< cant_jugadores; j++) //envio la cantidad de letras a los clientes
	{
		if((send(vectorfd[j], &cant_letras, sizeof(cant_letras), 0))==-1)
		{
			perror("\nerror al enviar la cantidad de letras:");
			exit(1);
		}
	}
}

void abrir_archivo(FILE* fp, char* path)
{
	if(!(fp = fopen(path, "rb"))) //abro el archivo que contiene las palabras
	{
		perror("\nerror al abrir archivo: ");
		exit(1);
	}
}

int generar_palabra(FILE* fp, char* palabra)
{
	int cant_lineas = cantidad_lineas_archivo(fp); //cantidad de lineas del archivo
	int num_aleatorio = generar_num_linea(cant_lineas); //genero numero aleatorio
	leer_linea(fp, num_aleatorio, palabra); //elijo linea en base al numero aleatorio
	int cant_letras = strlen(palabra); //cuento la cantidad de letras

	return cant_letras;
}
