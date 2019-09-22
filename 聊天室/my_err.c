#define _MY_ERR_C

#include "my_err.h"

#include <stdlib.h>
#include <stdio.h>

void my_err(char *err_string , int line) {
    fprintf(stdout,"line:%d",line);
    perror(err_string);
    exit(1);
}


