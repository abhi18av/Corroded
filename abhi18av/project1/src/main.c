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



int main (int argc, char* argv[])
{

    /* Read command line options */
    options_t options;
    options_parser(argc, argv, &options);


printf("Hello, World!\n");

int i;
double A[5] = {
    9.0, 2.9, 3.E+25, .00007,
};
// Doing some work
for (i = 0; i < 5; ++i)
{ printf("element␣%d␣is␣%g,␣\tits␣square␣is␣%g\n",
i,
A[i],
A[i]*A[i]);
}

#ifdef DEBUG
    fprintf(stdout, BLUE "Command line options:\n" NO_COLOR);
    fprintf(stdout, BROWN "help: %d\n" NO_COLOR, options.help);
    fprintf(stdout, BROWN "version: %d\n" NO_COLOR, options.version);
    fprintf(stdout, BROWN "use colors: %d\n" NO_COLOR, options.use_colors);
    fprintf(stdout, BROWN "filename: %s\n" NO_COLOR, options.file_name);
#endif


    /* Do your magic here :) */


    return EXIT_SUCCESS;
}

