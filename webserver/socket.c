#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h> 

int creer_serveur(int port) {
	int socket_serveur;
	int optval = 1;
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;	
	initialiser_signaux();
	socket_serveur = socket(AF_INET,SOCK_STREAM,0);
	if(socket_serveur == -1) {	
		perror("socket_serveur");
	}

	if(bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
		perror("bind socker_serveur");
	}

	if(listen(socket_serveur, 10) == -1) {
		perror("listen socket_serveur");
	}
	if (setsockopt(socket_serveur, SOL_SOCKET , SO_REUSEADDR , &optval , sizeof(int)) ==  -1){
	        perror("Can not set  SO_REUSEADDR  option");
	}
	return socket_serveur;
}

void initialiser_signaux(void){
  struct  sigaction  sa;
  sa.sa_handler = traitement_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  
  if (signal(SIGPIPE , SIG_IGN) ==  SIG_ERR)
    {
      perror("signal");
    }
  
  if (sigaction(SIGCHLD , &sa, NULL) ==  -1)
    {
      perror("sigaction(SIGCHLD)");
    }

}

void traitement_signal(int sig)
{
  printf("Signal %d reçu\n", sig);
  waitpid(-1,&sig,WNOHANG);
}

char *fgets_or_exit(char *buffer , int size , FILE *stream) {
  char * client;
  if((client=fgets(buffer,size,stream))== NULL){
    exit(1);
  }
  return client;
}
