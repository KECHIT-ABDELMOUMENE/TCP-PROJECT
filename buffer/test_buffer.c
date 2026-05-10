/* ABDELMOUMENE KECHIT 12509386
Je déclare qu'il s'agit de mon propre travail.
Ce travail a été réalisé intégralement par un être humain. */

#include <stdio.h>
#include "buffer.h"
int main(){
    buffer *b = buff_create(0,1024);
    char dest[512];
    while ((buff_fgets(b, dest,sizeof(dest))) != NULL){
        printf("read : %s",dest);
        fflush(stdout);
    }
    buff_free(b);
    return 0;
}