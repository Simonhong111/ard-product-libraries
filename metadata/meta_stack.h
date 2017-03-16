/*****************************************************************************
FILE: meta_stack.h
  
PURPOSE: Contains related defines and structures for pushing and popping
metadata node elements to the stack.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef META_STACK_H
#define META_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ard_error_handler.h"

/* Defines */
#define MAX_ARD_STACK_SIZE 1000

/* Prototypes */
int init_stack
(
    int *top_of_stack,  /* I: pointer to top of the stack; zero-based */
    char ***stack       /* I: pointer to the array of strings in the stack;
                              memory will be allocated for this pointer */
);

void free_stack
(
    char ***stack       /* I: pointer to the array of strings in the stack;
                              memory will be freed for this pointer */
);

int push
(
    int *top_of_stack,    /* I/O: pointer to top of the stack; zero-based */
    char **stack,         /* I/O: stack to push item to */
    const char *strval    /* I: string to push on stack */
);

char *pop
(
    int *top_of_stack,    /* I/O: pointer to top of the stack; zero-based */
    char **stack          /* I/O: stack to pop item from */
);

#endif
