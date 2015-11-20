#include"headers.h"
#define PUERTO 1529
#define MAX_JUG 8

struct dato{
  char nombre[15];
  int cantidad;
  int pid_cliente;
};
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

int funcion_crear_lista(struct nodo **,struct casilla);
int funcion_mensaje(int [],int);
void funcion_enviar_signal(int);

int main(void)
{
	int sockfd,cant,var2,vectorfd[MAX_JUG],x=0,var3,sig=SIGUSR1;
	struct casilla CASILLA;
	struct dato jugador[MAX_JUG];
	struct sockaddr_in servidor;
	struct sockaddr_in vector_cliente[MAX_JUG];
	struct nodo *inicio=NULL;
	struct nodo *vector_posicion[MAX_JUG];
	FILE *fp;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		printf("\nerror en socket\n");
		exit(1);
	}
	servidor.sin_family=AF_INET;
	servidor.sin_port=htons(PUERTO);
	servidor.sin_addr.s_addr=INADDR_ANY;
	memset(servidor.sin_zero,'\0',8);
	if((bind(sockfd,(struct sockaddr *)&servidor,sizeof(struct sockaddr)))==-1)
	{
		printf("\nerror en bind\n");
		exit(1);
	}
	if((listen(sockfd,MAX_JUG))==-1)
	{
		printf("\nerror en listen\n");
		exit(1);
	}
	printf("ingrese la cantidad de jugadores\n");
	scanf("%d",&cant);
	for(var3=0;var3<=cant-1;var3++)
	{
		vector_posicion[var3]=inicio;
	}
	__fpurge(stdin);
	system("clear");
	for(x=0;x<=cant-1;x++)
	{
		int c=sizeof(struct sockaddr);
		if((vectorfd[x]=accept(sockfd,(struct sockaddr *)&(vector_cliente[x]),&c))==-1)
		{
			printf("\nerror en accept\n");
			exit(1);
		}
		printf("jugador numero %d conectado\n",x+1);
	}
	funcion_mensaje(vectorfd,cant);
	for(var2=0;var2<=cant-1;var2++)
	{
	  if((recv(vectorfd[var2],jugador[var2].nombre,MAX_JUG,0))==-1)
	  {
	    jugador[var2].cantidad=2500;
	    printf("\nerror al recibir nombres\n");
	    exit(1);
	  }
	  if((recv(vectorfd[var2],&jugador[var2].pid_cliente,MAX_JUG,0))==-1)
	  {
	  	printf("\nerror al recibir los pids\n");
	  	exit(1);
	  }
	}
	system("clear");
	for(var2=0;var2<=cant-1;var2++)
	{
	  printf("se ha conectado %s\n",jugador[var2].nombre);
	}
	if(!(fp=fopen("monopolio.dbf","rb")))
	{
		printf("error al abrir el archivo\n");
		return 1;
	}
	fread(&CASILLA,sizeof(struct casilla),1,fp);
	while(!feof(fp))
	{
		funcion_crear_lista(&inicio,CASILLA);
		fread(&CASILLA,sizeof(struct casilla),1,fp);
	}
	for(;1;)
	{
		for(var3=0;var3<=cant-1;var3++)
		{
			if(jugador[var3].cantidad<=0)
			{
			  send(vectorfd[var3],&sig,4,0);
			}
		}
	}

}
int funcion_mensaje(int array[],int cantidad)
{
	char *mensaje="ingrese nombre\n";
	int var,var1;
	var1=strlen(mensaje);
	for(var=0;var<=cantidad-1;var++)
	{
		if((send(array[var],mensaje,var1,0))==-1)
		{
			printf("error al enviar el mensaje\n");
			return 1;
		}
	}
}
int funcion_crear_lista(struct nodo **ppio,struct casilla T)
{
	struct nodo *a,*b;
	if(!(b=(struct nodo *)malloc(sizeof(struct nodo))))
	{
		printf("error en funcion crear lista\n");
		return 1;
	}
	b->next=NULL;
	b->celda=T;
	if(*ppio==NULL)
	{
		*ppio=b;
		return 0;
	}
	for(a=*ppio;a->next!=NULL;a=a->next);
	a->next=b;
	return 0;
}
