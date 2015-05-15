//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "common.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

void Error(string message) {
    printf("Error: %s\n", message);
    getchar();
    exit(EXIT_FAILURE);
}
