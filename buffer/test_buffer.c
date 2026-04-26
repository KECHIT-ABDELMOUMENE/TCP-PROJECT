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