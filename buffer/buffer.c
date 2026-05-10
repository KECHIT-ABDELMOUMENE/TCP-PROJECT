/* ABDELMOUMENE KECHIT 12509386
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */

#include "buffer.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct buffer {
	int fd;
	char *buf;
	size_t buffsz;
	int unget;
	size_t start ;
	size_t end;
	int eof;
};

buffer *buff_create(int fd, size_t buffsz)
{	buffer *b = malloc(sizeof(buffer));
	if (b == NULL){
		return NULL ;
	}
	b->fd =fd ;
	b->buf = malloc(buffsz);
	if(b->buf == NULL){
		free(b);
		return NULL ;
	}
	b->buffsz = buffsz;
	b->unget = -1; //no char waiting
	b->start = 0;
	b->end = 0;
	b->eof =0;
	return b;
}

int buff_getc(buffer *b)
{
	if (b->unget != -1){

		int c = b->unget ;

		b->unget =-1 ;

		return c;

	}

	if (b->start == b->end){

	ssize_t n= read(b->fd,b->buf,b->buffsz);

	if (n <= 0){

		b->eof = 1;

		return EOF ;
	}
	b->start =0;
	b->end = n;
}
return (unsigned char)b->buf[b->start++];
}

int buff_ungetc(buffer *b, int c)
{	b->unget =  c ;
	return c ; 
}

void buff_free(buffer *b)
{	free(b->buf);
	free(b);
}

int buff_eof(const buffer *buff)
{	
	return buff->eof ;
}

int buff_ready(const buffer *buff)
{	
	return buff->start <buff->end ;
}

char *buff_fgets(buffer *b, char *dest, size_t size)
{	int i;
	for (i =0 ; i< size-1 ;i++){
	int c = buff_getc(b);
	if (c == EOF){
		break;
	}
	dest[i]=c;
	if (c == '\n'){ break ;}
}
dest[i]='\0';
if (i ==0 && buff_eof(b)){
	 return NULL ;
}
return dest ;
}

char *buff_fgets_crlf(buffer *b, char *dest, size_t size)
{
	int i ;
	 for (i =0 ; i< size -1 ; i++){
		int c = buff_getc(b);
		if (c==EOF){
			break;
		}
		dest[i]=c;
		if(c=='\r'){
			int next_char = buff_getc(b);
			if (next_char == '\n' && i+1 <size -1){
				i++;
				dest[i]='\n';
			}
			else if (next_char != EOF){
				buff_ungetc(b,next_char);
			}
			break ;
	 }
	}
	 dest[i]='\0';
	 if ( i==0 && buff_eof(b)){
		return NULL ;
	 }
	 return dest ;

}
