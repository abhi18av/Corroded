/**
 * See Copyright Notice in picrin.h
 */

#ifndef PICRIN_GC_H
#define PICRIN_GC_H

#if defined(__cplusplus)
extern "C" {
#endif

struct heap *pic_heap_open(pic_state *);
void pic_heap_close(pic_state *, struct heap *);

#if defined(__cplusplus)
}
#endif

#endif
