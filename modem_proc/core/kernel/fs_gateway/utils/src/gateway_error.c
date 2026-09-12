#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "gateway_error.h"

int gateway_error;

void gateway_error_set_code(int errcode){

   gateway_error = errcode;
   
}