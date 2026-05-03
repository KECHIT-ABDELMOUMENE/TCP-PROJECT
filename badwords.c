#include <stdio.h>
#include <string.h>
#include "badwords.h"

int is_badword(const char *pseaudonyme){
    for (int i = 0; badwords[i] != NULL; i++) {
        if (strcmp(pseaudonyme, badwords[i]) == 0) {
            return 1;
        }
    }
    return 0; 
}
