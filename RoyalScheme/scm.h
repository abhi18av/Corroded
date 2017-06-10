#ifndef SCM_H
#define SCM_H

#include <stdio.h>
#include <limits.h>

/************************* util.c *************************/

void scm_fatal(char *fmt, ...);

/************************ model.c *************************/

/* scm_int must be large enough to hold any Scheme fixnum.
 */
typedef long scm_int;

/* A scm_object can be any of Scheme's many types: boolean,
 * the empty list, number, symbol, pair, vector, etc.
 * For now we only have fixnums which will fit in a long.
 */
typedef long scm_object;

/* Trival conversions to and from fixnum since they are
 * the only type for now.
 */
#define scm_fixnum_make(val) ((scm_object)(val))
#define scm_fixnum_value(object) ((scm_int)(object))

/* Assume a two's complement hardware representation.
 *
 * Ensure that any number can be algebraically negated
 * and the result is still in the acceptable range.
 */
#define scm_fixnum_min ((((scm_int)1) \
    << (8 * sizeof(scm_object) - 1)) + 1)
#define scm_fixnum_max (-(scm_fixnum_min))

/************************* read.c *************************/

scm_object scm_read(FILE *in);

/************************* eval.c *************************/

scm_object scm_eval(scm_object expression);

/************************ print.c *************************/

int scm_write(FILE *out, scm_object object);

#endif
