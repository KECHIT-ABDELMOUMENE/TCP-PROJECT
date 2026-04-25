#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include "buffer/buffer.h"
#include "utils.h"
#include <string.h>

#define PORT_FREESCORD 4321

/** se connecter au serveur TCP d'adresse donnée en argument sous forme de
 * chaîne de caractère et au port donné en argument
 * retourne le descripteur de fichier de la socket obtenue ou -1 en cas
 * d'erreur. */
int connect_serveur_tcp(char *adresse, uint16_t port);

int main(int argc, char *argv[])
{	if (argc< 2){
		fprintf(stderr, "Error : missing IP address of the server\n");
		return -1;
}
	int fd_client = connect_serveur_tcp( argv[1] , PORT_FREESCORD);
	if (fd_client == -1){
		perror("connection to server");
		return -1;
	}
	char buffer[1024];
	ssize_t n;
	while (fgets(buffer, sizeof(buffer), stdin) != NULL){
		send(fd_client, buffer ,strlen(buffer), 0);
		n =recv(fd_client, buffer, sizeof(buffer), 0);
		write(1,buffer,n);
	}
	close(fd_client);
	return 0;
}

int connect_serveur_tcp(char *adresse, uint16_t port)
{
	/* pour éviter les warnings de variable non utilisée */
	int fd_client= socket(AF_INET, SOCK_STREAM, 0);
	if (fd_client == -1){
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family =AF_INET;
	addr.sin_port = htons(port);
	if (inet_pton(AF_INET, adresse, &addr.sin_addr) !=1 ){
		perror("inet_pton");
		close(fd_client);
		return -1;
	}
	
	if (connect( fd_client, (struct sockaddr *)&addr, sizeof(addr)) !=0){
		perror("connect");
		close(fd_client);
		return -1;
	}

	return fd_client;
}
