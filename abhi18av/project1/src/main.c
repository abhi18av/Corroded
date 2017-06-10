/*
 * ============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file of the project
 *
 *        Created:  03/24/2016 19:40:56
 *
 *         Author:  Gustavo Pantuza
 *   Organization:  Software Community
 *
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "colors.h"



int main ()
{



printf("Hello, World!\n");

int i;
double A[5] = { 1,2,3,4,5 };
// Doing some work
for (i = 0; i < 5; ++i)
{ printf("element␣%d␣is␣%g,␣\tits␣square␣is␣%g\n",
i,
A[i],
A[i]*A[i]);
}

    /* Do your magic here :) */


    return EXIT_SUCCESS;
}

