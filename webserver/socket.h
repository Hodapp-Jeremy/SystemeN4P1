#ifndef __SOCKET_H__
#define __SOCKET_H__
int creer_serveur(int port);
void initialiser_signaux(void);
void traitement_signal(int sig);
char *fgets_or_exit(char *buffer , int size , FILE *stream);
#endif
