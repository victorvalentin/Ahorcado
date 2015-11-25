#include"headers.h"
#define PUERTO 6000
#define MAX_JUG 8

int main(int argc,char *argv[])
{

	char mi_nombre[20];
	char palabra_teclado[20];
	char* nombre = malloc(20);
	char letra;

	char* mensaje_reci = malloc(20);
	int sockfd,op,cant_letras,var1,var3;
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
		perror("\nerror en connect: ");
		exit(1);
	}
	if((recv(sockfd,mensaje_reci,20,0))==-1) //recibo el mensaje del servidor
	{
		perror("\nerror al recibir el mensaje: ");
	}
	else
	{
		printf("%s\n",mensaje_reci);
		free(mensaje_reci);
		gets(mi_nombre);
		strcpy(nombre, mi_nombre);

		__fpurge(stdin);

		if((send(sockfd,nombre,20,0))==-1) //envio mi nombre
		{
		  perror("\nerror al enviar nombre\n");
		  exit(1);
		}

		free(nombre);
	}

	if((recv(sockfd, &cant_letras, sizeof(cant_letras), 0))==-1) //recibo la cantidad de letra
	{
		perror("\nerror al recibir la cantidad de letras\n");
		exit(1);
	}

	for(var3=0;var3<=cant_letras-1;var3++)
	{
		if((recv(sockfd,&op,sizeof(op),0))==-1) //a=0,b=1, cero si es mi turno para ingresar letra o palabra
	    {
	      printf("\nerror al recibir a o b\n");
		  exit(1);
	    }
	    if(op==0)
	    {
		  ETIQUETA:
		  __fpurge(stdin);
		  printf("1.insertar letra\n2.insertar palabra\n");
		  scanf("%d",&var1);
		  __fpurge(stdin);
		  switch(var1)
		  {
			    case 1:
	                if((send(sockfd,&var1,4,0))==-1)
					{
						printf("\nerror al enviar opcion\n");
						exit(1);
				    }
				    scanf("%c",&letra);__fpurge(stdin);
					if((send(sockfd,&letra,1,0))==-1)
					{
						printf("\nerror al enviar letra\n");
						exit(1);
					}
					break;
		    	case 2:
		    		if((send(sockfd,&var1,4,0))==-1)
		    		{
		    			printf("\error al enviar opcion\n");
		    			exit(1);
					}

		    		char* palabra = malloc(20);
		    		gets(palabra_teclado);
				    strcpy(palabra, palabra_teclado);
				    __fpurge(stdin);

				    if((send(sockfd,palabra,20,0))==-1)
					{
						printf("error al enviar palabra\n");
						exit(1);
					}

				    free(palabra);

				    break;
			    default :printf("\nopcion no valida...\nvuelva a ingresar\n");goto ETIQUETA;
		  }
	    }
	    else
	    {
	    	char* nombre_jugador = malloc(15);

	    	if((recv(sockfd,nombre_jugador,15,0))==-1)
	    	{
	    		printf("\nerror al recibir nombres\n");
	    		exit(1);
	    	}

	    	printf("esperando respuesta de %s\n",nombre_jugador);
	    	free(nombre_jugador);

		  if((recv(sockfd,&var1,4,0))==-1)
		  {
		  	printf("\nerror al recibir opcion del jugador\n");
		  	exit(1);
		  }

		  if(var1==1)
		  {
		  	if((recv(sockfd,&letra,1,0))==-1)
		  	{
		  		printf("error al recibir letra del jugador\n");
		  		exit(1);
			}

		  	printf("%c\n",letra);
		  }
		  else
		  {
			char* palabra_otro = malloc(20);

		  	if((recv(sockfd,palabra_otro,20,0))==-1)
		  	{
		  		printf("error al recibir palabra del jugador\n");
		  		exit(1);
			}
			printf("%s\n",palabra_otro);
		  }
	    }
	}

	return 0;
}

