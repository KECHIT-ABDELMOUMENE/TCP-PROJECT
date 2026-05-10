/* ABDELMOUMENE KECHIT 12509386
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */


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
#include "utils.h"
#include "badwords.h"
#define PORT_FREESCORD 4321

/** Gérer toutes les communications avec le client renseigné dans
 * user, qui doit être l'adresse d'une struct user */
void *handle_client(void *user);
/** Créer et configurer une socket d'écoute sur le port donné en argument
 * retourne le descripteur de cette socket, ou -1 en cas d'erreur */
int create_listening_sock(uint16_t port);
void *repeater(void *arg);
int tube[2];
struct list *users;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
int main(int argc, char *argv[])
{	
	int fd_serveur = create_listening_sock(PORT_FREESCORD);
	if(fd_serveur == -1){
		return -1;
	}
	pipe(tube);
	users = list_create();//pour creer une liste 
	pthread_t r ;//r pour répéteur
	pthread_create(&r,NULL,repeater,NULL);
	pthread_detach(r);

	while(1){
	struct user *u = user_accept(fd_serveur);
	if (u == NULL){
		perror("ACCEPT");
		continue ;
	}
	pthread_mutex_lock(&mutex);
	list_add(users,u);
	pthread_mutex_unlock(&mutex);
	
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

	send(u->sock,"Bienvenue sur Freescord !\r\n", 27,0);

	send(u->sock, "\r\n", 2, 0);

	while(1){
		memset(buffer,0,sizeof(buffer));
		n=recv(u->sock ,buffer,sizeof(buffer),0);
		if (n<=0){
			break ;
		}

		crlf_to_lf(buffer);

		buffer[strcspn(buffer,"\n")]='\0';
		
		if(strncmp(buffer ,"nickname ",9) !=0){
			send(u->sock,"3 la commande ne commence pas par nickname\r\n",43,0);
			continue;
		}
		char *pseudonyme = buffer + 9;
		if(strlen(pseudonyme) >16){
			send(u->sock,"2 le nickname est troplong\r\n",28,0);
			continue;
		}
		if(strchr(pseudonyme, ':')){
			send(u->sock ,"2 Nickname is forbidden\r\n",25,0);
			continue;
		}

		if (is_badword(pseudonyme)) { 
    	send(u->sock, "2 Nickname is forbidden\r\n", 25, 0); 
    	continue; 
		}
		int tmp=0;
		pthread_mutex_lock(&mutex);
		
		for(struct node *current = users->first ; current != NULL; current = current->next){
			struct user *other_user_nickname = current->elt;
			if (other_user_nickname != u && strcmp(other_user_nickname->nickname, pseudonyme) == 0) {
				tmp = 1;
				break;
			}
		}

		pthread_mutex_unlock(&mutex);
		if(tmp){
			send(u->sock,"1 Nickname is already used\r\n",29,0);
			continue;
		}
		strncpy(u->nickname,pseudonyme,16);
		u->nickname[16] = '\0';
		send(u->sock,"0 Nickname accepted\r\n",21,0);
		break;
	}
	
	while((n=recv(u->sock ,buffer,sizeof(buffer),0))>0){
		if (n>512){
			continue ;
		}
		buffer[n]='\0';
		crlf_to_lf(buffer);
		char message[1100];
		snprintf(message,sizeof(message),"%s: %s",u->nickname,buffer);
		lf_to_crlf(message);
		write(tube[1],message,strlen(message));
	}

	pthread_mutex_lock(&mutex);
	list_remove_element(users,u);
	pthread_mutex_unlock(&mutex);
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
	int p= 1;
	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&p, sizeof(p));

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

void *repeater(void *arg){
	char buffer[1024];
	ssize_t n;
	while ((n= read(tube[0],buffer, sizeof(buffer)-1))>0){

		buffer[n]='\0';
		
		pthread_mutex_lock(&mutex);

		struct node *curr = users->first;
		
		size_t len = strlen(buffer);

  	for (; curr != NULL; curr = curr -> next) {

		struct user *u = (struct user *)curr->elt;

 		send(u->sock,buffer,len,0);
  	}

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}
