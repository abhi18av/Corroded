/**
 * Copyright (c) 2013-2016 Picrin developers.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PICRIN_H
#define PICRIN_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
#include <limits.h>
#include <stdarg.h>

#include "picrin/setup.h"

typedef struct pic_state pic_state;

#if PIC_NAN_BOXING
# include <stdint.h>
typedef uint64_t pic_value;
#else
typedef struct {
  unsigned char type;
  union {
    void *data;
    double f;
    int i;
    char c;
  } u;
} pic_value;
#endif

typedef void *(*pic_allocf)(void *userdata, void *ptr, size_t n);

pic_state *pic_open(pic_allocf f, void *userdata);
void pic_close(pic_state *);

int pic_get_args(pic_state *, const char *fmt, ...);

void *pic_malloc(pic_state *, size_t);
void *pic_realloc(pic_state *, void *, size_t);
void *pic_calloc(pic_state *, size_t, size_t);
void pic_free(pic_state *, void *);

typedef pic_value (*pic_func_t)(pic_state *);

void *pic_alloca(pic_state *, size_t);
size_t pic_enter(pic_state *);
void pic_leave(pic_state *, size_t);
pic_value pic_protect(pic_state *, pic_value);
void pic_gc(pic_state *);

void pic_add_feature(pic_state *, const char *feature);

void pic_defun(pic_state *, const char *name, pic_func_t f);
void pic_defvar(pic_state *, const char *name, pic_value v, pic_value conv);

void pic_define(pic_state *, const char *lib, const char *name, pic_value v);
pic_value pic_ref(pic_state *, const char *lib, const char *name);
void pic_set(pic_state *, const char *lib, const char *name, pic_value v);
pic_value pic_closure_ref(pic_state *, int i);
void pic_closure_set(pic_state *, int i, pic_value v);
pic_value pic_funcall(pic_state *, const char *lib, const char *name, int n, ...);
pic_value pic_make_var(pic_state *, pic_value init, pic_value conv);

pic_value pic_return(pic_state *, int n, ...);
pic_value pic_vreturn(pic_state *, int n, va_list);
pic_value pic_valuesk(pic_state *, int n, pic_value *retv);
int pic_receive(pic_state *, int n, pic_value *retv);

void pic_make_library(pic_state *, const char *lib);
void pic_in_library(pic_state *, const char *lib);
bool pic_find_library(pic_state *, const char *lib);
const char *pic_current_library(pic_state *);
void pic_import(pic_state *, const char *lib);
void pic_export(pic_state *, pic_value sym);

typedef void (*pic_panicf)(pic_state *, const char *msg);

pic_panicf pic_atpanic(pic_state *, pic_panicf f);
PIC_NORETURN void pic_panic(pic_state *, const char *msg);
PIC_NORETURN void pic_error(pic_state *, const char *msg, int n, ...);
PIC_NORETURN void pic_raise(pic_state *, pic_value v);
pic_value pic_make_error(pic_state *, const char *type, const char *msg, pic_value irrs);

pic_value pic_lambda(pic_state *, pic_func_t f, int n, ...);
pic_value pic_vlambda(pic_state *, pic_func_t f, int n, va_list);
pic_value pic_call(pic_state *, pic_value proc, int, ...);
pic_value pic_vcall(pic_state *, pic_value proc, int, va_list);
pic_value pic_apply(pic_state *, pic_value proc, int n, pic_value *argv);
pic_value pic_applyk(pic_state *, pic_value proc, int n, pic_value *argv);

typedef struct xFILE xFILE;

pic_value pic_open_port(pic_state *, xFILE *file);
xFILE *pic_fileno(pic_state *, pic_value port);
void pic_close_port(pic_state *, pic_value port);

int pic_int(pic_state *, pic_value i);
double pic_float(pic_state *, pic_value f);
char pic_char(pic_state *, pic_value c);
#define pic_bool(pic,b) (! pic_false_p(pic, b))
const char *pic_str(pic_state *, pic_value str);
#define pic_sym(pic,s) (pic_str(pic, pic_sym_name(pic, (s))))
unsigned char *pic_blob(pic_state *, pic_value blob, int *len);
void *pic_data(pic_state *, pic_value data);

typedef struct {
  const char *type_name;
  void (*dtor)(pic_state *, void *);
  void (*mark)(pic_state *, void *, void (*)(pic_state *, pic_value));
} pic_data_type;

pic_value pic_invalid_value(pic_state *);
pic_value pic_undef_value(pic_state *);
pic_value pic_int_value(pic_state *, int);
pic_value pic_float_value(pic_state *, double);
pic_value pic_char_value(pic_state *, char);
pic_value pic_true_value(pic_state *);
pic_value pic_false_value(pic_state *);
#define pic_bool_value(pic, b) ((b) ? pic_true_value(pic) : pic_false_value(pic))
pic_value pic_eof_object(pic_state *);
pic_value pic_str_value(pic_state *, const char *str, int len);
#define pic_cstr_value(pic, cstr) pic_str_value(pic, (cstr), strlen(cstr))
#define pic_lit_value(pic, lit) pic_str_value(pic, "" lit, -((int)sizeof lit - 1))
pic_value pic_strf_value(pic_state *, const char *fmt, ...);
pic_value pic_vstrf_value(pic_state *, const char *fmt, va_list ap);
pic_value pic_blob_value(pic_state *, const unsigned char *buf, int len);
pic_value pic_data_value(pic_state *, void *ptr, const pic_data_type *type);

enum {
  PIC_TYPE_INVALID = 1,
  PIC_TYPE_FLOAT   = 2,
  PIC_TYPE_INT     = 3,
  PIC_TYPE_CHAR    = 4,
  PIC_TYPE_EOF     = 5,
  PIC_TYPE_UNDEF   = 6,
  PIC_TYPE_TRUE    = 8,
  PIC_TYPE_NIL     = 7,
  PIC_TYPE_FALSE   = 9,
  PIC_IVAL_END     = 10,
/* -------------------- */
  PIC_TYPE_STRING  = 16,
  PIC_TYPE_VECTOR  = 17,
  PIC_TYPE_BLOB    = 18,
  PIC_TYPE_PORT    = 20,
  PIC_TYPE_ERROR   = 21,
  PIC_TYPE_ID      = 22,
  PIC_TYPE_ENV     = 23,
  PIC_TYPE_DATA    = 24,
  PIC_TYPE_DICT    = 25,
  PIC_TYPE_WEAK    = 26,
  PIC_TYPE_RECORD  = 27,
  PIC_TYPE_SYMBOL  = 28,
  PIC_TYPE_PAIR    = 29,
  PIC_TYPE_CXT     = 30,
  PIC_TYPE_CP      = 31,
  PIC_TYPE_FUNC    = 32,
  PIC_TYPE_IREP    = 33
};

#define pic_invalid_p(pic,v) (pic_type(pic,v) == PIC_TYPE_INVALID)
#define pic_undef_p(pic,v) (pic_type(pic,v) == PIC_TYPE_UNDEF)
#define pic_int_p(pic,v) (pic_type(pic,v) == PIC_TYPE_INT)
#define pic_float_p(pic,v) (pic_type(pic,v) == PIC_TYPE_FLOAT)
#define pic_char_p(pic,v) (pic_type(pic,v) == PIC_TYPE_CHAR)
#define pic_eof_p(pic, v) (pic_type(pic, v) == PIC_TYPE_EOF)
#define pic_true_p(pic,v) (pic_type(pic,v) == PIC_TYPE_TRUE)
#define pic_false_p(pic,v) (pic_type(pic,v) == PIC_TYPE_FALSE)
#define pic_id_p(pic, v) (pic_type(pic, v) == PIC_TYPE_ID || pic_type(pic, v) == PIC_TYPE_SYMBOL)
#define pic_str_p(pic,v) (pic_type(pic,v) == PIC_TYPE_STRING)
#define pic_blob_p(pic,v) (pic_type(pic,v) == PIC_TYPE_BLOB)
#define pic_proc_p(pic,v) (pic_type(pic,v) == PIC_TYPE_FUNC || pic_type(pic, v) == PIC_TYPE_IREP)
#define pic_nil_p(pic,v) (pic_type(pic,v) == PIC_TYPE_NIL)
#define pic_pair_p(pic,v) (pic_type(pic,v) == PIC_TYPE_PAIR)
#define pic_vec_p(pic,v) (pic_type(pic,v) == PIC_TYPE_VECTOR)
#define pic_dict_p(pic,v) (pic_type(pic,v) == PIC_TYPE_DICT)
#define pic_weak_p(pic,v) (pic_type(pic,v) == PIC_TYPE_WEAK)
#define pic_port_p(pic, v) (pic_type(pic, v) == PIC_TYPE_PORT)
#define pic_sym_p(pic,v) (pic_type(pic,v) == PIC_TYPE_SYMBOL)
bool pic_data_p(pic_state *, pic_value, const pic_data_type *);

int pic_type(pic_state *, pic_value);
const char *pic_typename(pic_state *, int);

bool pic_eq_p(pic_state *, pic_value, pic_value);
bool pic_eqv_p(pic_state *, pic_value, pic_value);
bool pic_equal_p(pic_state *, pic_value, pic_value);

/* pair */
pic_value pic_cons(pic_state *, pic_value car, pic_value cdr);
pic_value pic_car(pic_state *, pic_value pair);
pic_value pic_cdr(pic_state *, pic_value pair);
void pic_set_car(pic_state *, pic_value pair, pic_value car);
void pic_set_cdr(pic_state *, pic_value pair, pic_value cdr);
pic_value pic_caar(pic_state *, pic_value);
pic_value pic_cadr(pic_state *, pic_value);
pic_value pic_cdar(pic_state *, pic_value);
pic_value pic_cddr(pic_state *, pic_value);

/* list */
pic_value pic_nil_value(pic_state *);
bool pic_list_p(pic_state *, pic_value);
pic_value pic_make_list(pic_state *, int n, pic_value *argv);
pic_value pic_list(pic_state *, int n, ...);
pic_value pic_vlist(pic_state *, int n, va_list);
pic_value pic_list_ref(pic_state *, pic_value list, int i);
void pic_list_set(pic_state *, pic_value list, int i, pic_value v);
pic_value pic_list_tail(pic_state *, pic_value list, int i);
int pic_length(pic_state *, pic_value list);
pic_value pic_reverse(pic_state *, pic_value list);
pic_value pic_append(pic_state *, pic_value xs, pic_value ys);

/* vector */
pic_value pic_make_vec(pic_state *, int n, pic_value *argv);
pic_value pic_vec_ref(pic_state *, pic_value vec, int i);
void pic_vec_set(pic_state *, pic_value vec, int i, pic_value v);
int pic_vec_len(pic_state *, pic_value vec);

/* dictionary */
pic_value pic_make_dict(pic_state *);
pic_value pic_dict_ref(pic_state *, pic_value dict, pic_value key);
void pic_dict_set(pic_state *, pic_value dict, pic_value key, pic_value);
void pic_dict_del(pic_state *, pic_value dict, pic_value key);
bool pic_dict_has(pic_state *, pic_value dict, pic_value key);
int pic_dict_size(pic_state *, pic_value dict);
bool pic_dict_next(pic_state *, pic_value dict, int *iter, pic_value *key, pic_value *val);

/* ephemeron */
pic_value pic_make_weak(pic_state *);
pic_value pic_weak_ref(pic_state *, pic_value weak, pic_value key);
void pic_weak_set(pic_state *, pic_value weak, pic_value key, pic_value val);
void pic_weak_del(pic_state *, pic_value weak, pic_value key);
bool pic_weak_has(pic_state *, pic_value weak, pic_value key);

/* symbol */
pic_value pic_intern(pic_state *, pic_value str);
#define pic_intern_str(pic,s,i) pic_intern(pic, pic_str_value(pic, (s), (i)))
#define pic_intern_cstr(pic,s) pic_intern(pic, pic_cstr_value(pic, (s)))
#define pic_intern_lit(pic,lit) pic_intern(pic, pic_lit_value(pic, lit))
pic_value pic_sym_name(pic_state *, pic_value sym);

/* string */
int pic_str_len(pic_state *, pic_value str);
char pic_str_ref(pic_state *, pic_value str, int i);
pic_value pic_str_cat(pic_state *, pic_value str1, pic_value str2);
pic_value pic_str_sub(pic_state *, pic_value str, int i, int j);
int pic_str_cmp(pic_state *, pic_value str1, pic_value str2);
int pic_str_hash(pic_state *, pic_value str);



/* External I/O */

#define XSEEK_CUR 0
#define XSEEK_END 1
#define XSEEK_SET 2

xFILE *xfunopen(pic_state *, void *cookie, int (*read)(pic_state *, void *, char *, int), int (*write)(pic_state *, void *, const char *, int), long (*seek)(pic_state *, void *, long, int), int (*close)(pic_state *, void *));
size_t xfread(pic_state *, void *ptr, size_t size, size_t count, xFILE *fp);
size_t xfwrite(pic_state *, const void *ptr, size_t size, size_t count, xFILE *fp);
long xfseek(pic_state *, xFILE *fp, long offset, int whence);
int xfclose(pic_state *, xFILE *fp);

void xclearerr(pic_state *, xFILE *fp);
int xfeof(pic_state *, xFILE *fp);
int xferror(pic_state *, xFILE *fp);

int xfputc(pic_state *, int c, xFILE *fp);
int xfgetc(pic_state *, xFILE *fp);
int xfputs(pic_state *, const char *s, xFILE *fp);
char *xfgets(pic_state *, char *s, int size, xFILE *fp);
int xungetc(pic_state *, int c, xFILE *fp);
int xfflush(pic_state *, xFILE *fp);

int xfprintf(pic_state *, xFILE *fp, const char *fmt, ...);
int xvfprintf(pic_state *, xFILE *fp, const char *fmt, va_list);


#if defined(__cplusplus)
}
#endif

#endif
