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


	buffer *b = buff_create(fd_client, 1024);

	struct pollfd fds[2];

	fds[0].fd =0 ;

	fds[0].events =POLLIN ;

	fds[1].fd = fd_client;

	fds[1].events =POLLIN;

	char line[514];

	while (1){

		int timeout = buff_ready(b) ? 0: -1 ;

		poll(fds ,2 , timeout);

		if(fds[0].revents & POLLIN){
			ssize_t n = read(0 ,line , sizeof(line)-1);
			if (n<=0){ 
				break ;
			}
			line[n]='\0';
			lf_to_crlf(line);
			send(fd_client, line, strlen(line),0);

		}

		if ((fds[1].revents & POLLIN) || buff_ready(b)){
			if (buff_fgets_crlf(b,line,sizeof(line)) == NULL){
				break ;
			}
			crlf_to_lf(line);
			printf("%s",line);
			fflush(stdout);
		}

	}

	buff_free(b);

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
