#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  int serveur = creer_serveur(8080);
  int socket_client;
  int pid;
  char msg[256];
  char* token;
  int cptToken=0;
  int nbMot = 0;
  const char *message_bienvenue = "Bonjour , bienvenue sur mon serveur\n";
  while((socket_client = accept(serveur,NULL,NULL)) != -1){
    pid = fork();
    if(pid == 0){
      FILE *fichier;
      fichier = fdopen(socket_client,"w+");
      if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1){
 	 perror("erreur write");
      }
      if(fgets(msg,256,fichier) != NULL){
	token = strtok(msg, " ");
	while(token){
	  cptToken++;
	  if(strcmp(token,"GET") == 0 && cptToken == 1){
	    nbMot++;
	  }
	  if(strcmp(token,"/") == 0 && cptToken == 2){
	    nbMot++;
	  }
	  if((strcmp(token,"HTTP/1.1\r\n") == 0 || strcmp(msg,"HTTP/1.0\r\n") == 0) && cptToken == 3){
	    nbMot++;
	  }
	  token = strtok(NULL," ");
	}
	if(nbMot == 3){
	  printf("la premiere ligne est correcte\n");
	}else{
	  printf("Faux !!!");
	}	
      }
      fclose(fichier);
      exit(EXIT_SUCCESS);
    }
    else{
      close(socket_client);
    }
  }
  return 1;
}

