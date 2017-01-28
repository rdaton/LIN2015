#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <err.h>
#include <errno.h>

#define MAX_MESSAGE_SIZE 128

char* nombre_programa=NULL;


typedef enum {
    NORMAL_MSG, /* Mensaje para transferir lineas de la conversacion entre
    ambos usuarios del chat */
    USERNAME_MSG, /* Tipo de mensaje reservado para enviar el nombre de
    usuario al otro extremo*/
    END_MSG /* Tipo de mensaje que se envía por el FIFO cuando un extremo
    finaliza la comunicación */
}message_type_t;

struct fifo_message{
    char data[MAX_MESSAGE_SIZE]; //Cadena de caracteres (acabada en '\0)
    message_type_t type;
    unsigned int nr_bytes;
};

typedef struct mensaje{
  char* direccion_escritor;
  char* direccion_lector;
  char* nombre_usuario;
} mensaje_inf; 



/*
struct fifo_message {
	unsigned int nr_bytes;
	char data[MAX_MESSAGE_SIZE];
};*/


//static void fifo_send (const char* path_fifo) {
static void fifo_send (void *threadid) {

mensaje_inf* mensaje_variable=(char*)threadid;
char* path_send = mensaje_variable.direccion_escritor;

  struct fifo_message nombreUsuario;
  nombreUsuario.data=mensaje_variable.nombre_usuario;
  nombreUsuuario.type=USERNAME_MSG;

  struct fifo_message message;
  message.type=NORMAL_MSG;
  int fd_fifo=0;
  int bytes=0,wbytes=0;
  const int size2=sizeof(nombreUsuario)-sizeof(struct fifo_message.nr_bytes));
  const int size=sizeof(struct fifo_message);

  
  //manda una vez el nombre de usuario
   fd_fifo=open(path_send,O_WRONLY);

  if (fd_fifo<0) {
  perror(path_send);
  exit(1);
  }
     
  nombreUsuario.nr_bytes=size2;
  wbytes=write(fd_fifo,&nombreUsuario,size2);

  if (wbytes > 0 && wbytes!=size2) {
    fprintf(stderr,"Can't write the whole register\n");
    exit(1);
    }else if (wbytes < 0){
    perror("Error when writing to the FIFO\n");
    exit(1);
    }   
  }
  close(fd_fifo);

/* Bucle de envío de datos a través del FIFO
    - Leer de la entrada estandar hasta fin de fichero
 */
  
  fd_fifo=open(path_send,O_WRONLY);

  if (fd_fifo<0) {
  perror(path_send);
  exit(1);
  }
  while((bytes=read(0,message.data,MAX_MESSAGE_SIZE))>0) {
	message.nr_bytes=bytes;
	wbytes=write(fd_fifo,&message,size+sizeof(struct fifo_message.type));

	if (wbytes > 0 && wbytes!=size) {
		fprintf(stderr,"Can't write the whole register\n");
		exit(1);
  	}else if (wbytes < 0){
		perror("Error when writing to the FIFO\n");
		exit(1);
  	}		
  }
  

  if (bytes < 0) {
	fprintf(stderr,"Error when reading from stdin\n");
	exit(1);
  }
  
  close(fd_fifo);
}

//static void fifo_receive (const char* path_fifo) {
static void fifo_receive (void *threadid) {

mensaje_inf* mensaje_variable=(char*)threadid;
char* path_leer = mensaje_variable.direccion_lector;

  //char* path_leer=(char*)threadid;
  //struct fifo_message message;
  int fd_fifo=0;
  int bytes=0,wbytes=0;
  const int size=sizeof(struct fifo_message);

  fd_fifo=open(path_leer,O_RDONLY);

  if (fd_fifo<0) {
	perror(path_leer);
	exit(1);
  }


  while((bytes=read(fd_fifo,&message,size))==size) {
	/* Write to stdout */
	wbytes=write(1,message.data,message.nr_bytes);
	
	if (wbytes!=message.nr_bytes) {
		fprintf(stderr,"Can't write data to stdout\n");
		exit(1);
  	}	
 }

  if (bytes > 0){
	fprintf(stderr,"Can't read the whole register\n");
	exit(1);
  }else if (bytes < 0) {
	fprintf(stderr,"Error when reading from the FIFO\n");
	exit(1);
  }
	
   close(fd_fifo);
}


/*
static void
uso (int status)
{
  if (status != EXIT_SUCCESS)
    warnx("Pruebe `%s -h' para obtener mas informacion.\n", nombre_programa);
  else
    {
      printf ("Uso: %s -f <path_fifo> [OPCIONES]\n", nombre_programa);
fputs ("\
  -r,  el proceso actúa como receptor de los mensajes el FIFO\n\
  -s,  el proceso envía los mensajes leidos de la entrada estandar por el FIFO\n\
", stdout);
      fputs ("\
  -h,	Muestra este breve recordatorio de uso\n\
", stdout);
    }
    exit (status);
}
*/



int main (int argc, char **argv)
{

char* path_fifo0_lector=NULL;
char* path_fifo1_escritor=NULL;

mensaje_inf mensaje_inf_instancia;

mensaje_inf_instancia.nombre_usuario=argv[1];
mensaje_inf_instancia.direccion_lector=argv[2];
mensaje_inf_instancia.direccion_escritor=argv[3];

nombre_programa = argv[0]; 
//path_fifo0_lector=[2];
//path_fifo1_escritor=argv[3];


    pthread_t lector;
    pthread_t escritor;
    int rc;
    long t1=1;
    long t2=2;


    
    //for(t=0; t<NUM_THREADS; t++){
    printf("In main: creating thread hello\n");
    rc = pthread_create(&lector, NULL, fifo_receive, (void *)mensaje_inf_instancia);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      }
    rc = pthread_create(&escritor, NULL, fifo_send, (void *)mensaje_inf_instancia);
      if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
      } 

  (void)pthread_join(lector,NULL);
  (void)pthread_join(escritor,NULL);

  /*int optc;
  char* path_fifo=NULL;
  int receive=0;


  while ((optc = getopt (argc, argv, "srhf:")) != -1)
    {
      switch (optc)
	{

	case 'h':
	  uso(EXIT_SUCCESS);
	  break;
	
	case 'r':
	  receive=1;
	  break;

	case 's':
	  receive=0;
	  break;	

	case 'f':
	  path_fifo=optarg;
	  break;	

	default:
	  uso (EXIT_FAILURE);
	}
    }

 if (!path_fifo)
	uso(EXIT_FAILURE);

 
  //if (receive)
	fifo_receive(path_fifo);
  //else
	fifo_send(path_fifo);
  */

  exit (EXIT_SUCCESS);
}
