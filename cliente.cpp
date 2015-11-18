#include"headers.h"
#define PUERTO 1529
#define MAX_JUG 8

struct casilla{
	int tipo; //0.si es un avenida,1.si esa arca comunal,2.si es suerte,3.si es impuest,4.si es ferrocaril,5.carcel,6.servicio,7.valla a la carcel,8.libre estacionamientoy 9 salida
	char nombre[50];
	int precio;
	int cantidad_de_casas;
	int cantidad_de_hoteles;
	int situacion; //0.si esta hipotecada,1.si no esta hipotecada
	char propietario[15];
};
struct nodo{                      //estructura autoreferenciada
	struct casilla celda;
	struct nodo *next;
};

void funcion_trapeo(int);

int main(int argc,char *argv[])
{
	if((signal(SIGUSR1,funcion_trapeo))==SIG_ERR)
	{
		printf("\nerror en signal\n");
		exit(1);
	}
	char mensaje_reci[20],nombre[20];
	int sockfd,op,mi_pid=getpid(),signal;
	struct nodo *mi_lista=NULL;
	struct hostent *cliente;
	struct sockaddr_in mi_servidor;
	if((cliente=gethostbyname((const char *)argv[1]))==NULL)
	{
		printf("\nerror en gethostbyname\n");
		exit(1);
	}
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("\nerror en socket\n");
		exit(1);
	}
	mi_servidor.sin_family=AF_INET;
	mi_servidor.sin_port=htons(PUERTO);
	mi_servidor.sin_addr=*((struct in_addr *)cliente->h_addr);
	memset(mi_servidor.sin_zero,'\0',8);
	if((connect(sockfd,(struct sockaddr *)&mi_servidor,sizeof(struct sockaddr)))==-1)
	{
		printf("\nerror en connect\n");
		exit(1);
	}
	if((recv(sockfd,mensaje_reci,20,0))==-1)
	{
		printf("\nerror al recibir el mensaje");
	}
	else
	{
		printf("%s",mensaje_reci);
		gets(nombre);
		__fpurge(stdin);
		if((send(sockfd,nombre,20,0))==-1)
		{
		  printf("\nerror al enviar nombre\n");
		  exit(1);
		}
		if((send(sockfd,&mi_pid,4,0))==-1)
		{
			printf("\nerror al enviar pid\n");
			exit(1);
		}
	}
	if((recv(sockfd,&signal,4,0))==-1)
	{
	  printf("error al recibir señal\n");
	  exit(1);
	}
	else
	{
	  raise(signal);
	}
	
	
}
void funcion_trapeo(int sig)
{
        int dado;
	srand(time(NULL));
	dado=1+rand()%11;
	printf("\n0.terminar turno\n1.lanzar dados");
}

