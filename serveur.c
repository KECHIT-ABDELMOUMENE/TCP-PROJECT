#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list/list.h"
#include "user.h"
#include<netinet/in.h>
#include <string.h>
#define PORT_FREESCORD 4321

/** Gérer toutes les communications avec le client renseigné dans
 * user, qui doit être l'adresse d'une struct user */
void *handle_client(void *user);
/** Créer et configurer une socket d'écoute sur le port donné en argument
 * retourne le descripteur de cette socket, ou -1 en cas d'erreur */
int create_listening_sock(uint16_t port);

int main(int argc, char *argv[])
{	
	int fd_serveur = create_listening_sock(PORT_FREESCORD);
	if(fd_serveur == -1){
		return -1;
	}
	while(1){
	struct user *u = user_accept(fd_serveur);
	if (u == NULL){
		perror("ACCEPT");
		continue ;
	}
	pthread_t t;
	pthread_create(&t,NULL, handle_client,u);
	pthread_detach(t);
}
	close(fd_serveur);
	return 0;
}

void *handle_client(void *clt)
{	
	struct user *u =(struct user *)clt ; 
	char buffer[1024];
	ssize_t n;
	while((n=recv(u->sock ,buffer,sizeof(buffer),0))>0){
		send(u->sock,buffer,n,0);
	}
	user_free(u);
	return NULL;
}

int create_listening_sock(uint16_t port)
{
	int fd= socket(AF_INET, SOCK_STREAM, 0);
	if (fd== -1){
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));// pour éviter les problèmes de padding
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1){
		perror("bind");
		close(fd);
		return -1;
	}
	if (listen(fd, 10) == -1){
		perror("listen");
		close(fd);
		return -1;
	}	
	return fd;
}
