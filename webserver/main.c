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
	char* messageRequest;
	const char *message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";	
	initialiser_signaux();
	while((socket_client = accept(serveur,NULL,NULL)) != -1){
		if(socket_client == -1){
			perror("Socket client");
			return -1;
		}
		pid = fork();
		if(pid == 0){
			FILE *fichier;
			fichier = fdopen(socket_client,"w+");
			if(fichier == NULL){
				perror("fdopen erreur");
				return -1;
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
				if(nbMot != 3){
					messageRequest = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n\n400 Bad Request\r\n";
					write(socket_client,messageRequest,strlen(messageRequest));
					return -1;
				}
				if(fgets(msg,256,fichier) != NULL){
					while(strcmp(msg,"\n") != 0 && strcmp(msg,"\r\n") != 0){
						printf("%s\n",msg);
						fgets(msg,256,fichier);
					}
				}	
				messageRequest = "HTTP/1.1 200 OK\r\nContent-Length: %d \r\n";
				fprintf(fichier,messageRequest,strlen(messageRequest));
				if(write(socket_client, message_bienvenue, strlen(message_bienvenue)) == -1){
					perror("erreur write");
				}
			}else{
				perror("message null");
				return -2;
			}
			fclose(fichier);
			exit(EXIT_SUCCESS);
		}else{
			close(socket_client);
		}
	}
	return 1;
}

