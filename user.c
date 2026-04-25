#include "user.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include<unistd.h>

/** accepter une connection TCP depuis la socket d'écoute sl et retourner un
 * pointeur vers un struct user, dynamiquement alloué et convenablement
 * initialisé */
struct user *user_accept(int sl)
{
	struct user *u = malloc(sizeof(struct user));
	if (u==NULL){
		return NULL;
	}
	u->addr_len = (sizeof(struct sockaddr_in));
	u->address = malloc(sizeof(struct sockaddr_in ));

	if (u->address== NULL){
		free(u);
		return NULL;
	}

	u->sock = accept(sl, u->address , &u->addr_len);
	if (u->sock == -1){
		free(u->address);
		free(u);
		return NULL ;
	}

	
	return u;
	/* pour éviter les warnings de variable non utilisée */
	
}

/** libérer toute la mémoire associée à user */
void user_free(struct user *user)
{	
	free(user -> address);
	close(user ->sock);
	free(user);
}