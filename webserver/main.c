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
	int taille = 0;
	char msg[256]="";
	const char *message_bienvenue = "Bonjour , bienvenue sur mon serveur\n";
	while((socket_client = accept(serveur,NULL,NULL)) != -1){
		pid = fork();
		if(pid == 0){
			sleep(1);
			write(socket_client, message_bienvenue, strlen(message_bienvenue));
			}
		while((taille = read(socket_client, msg,256)) != -1){
			if(write(socket_client, msg, strlen(msg)) == -1){
				perror("Erreur ecriture");
			}
		}
	}
	return 1;
}

