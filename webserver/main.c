#include <stdio.h>
#include <string.h>
#include "socket.h"

int main() {
	int serveur = creer_serveur(8080);
	return serveur;
}

