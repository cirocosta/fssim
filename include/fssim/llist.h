#ifndef FSSIM__LLIST_H
#define FSSIM__LLIST_H

#include "fssim/common.h"

typedef void (*fs_llist_destructor)(void*);

typedef struct fs_llist_t {
  struct fs_llist_t* next;
  void* data;
} fs_llist_t;

/**
 * Creates a llist.
 */
fs_llist_t* fs_llist_create(void* data);

/**
 * Destroys a list
 */
void fs_llist_destroy(fs_llist_t* list, fs_llist_destructor destructor);

/**
 * Removes the list from the others
 * (a)->(b)->(c)->(d)->NIL
 *     remove(b)
 *  (a)->(b)->NIL
 *  (c)->(d)->NIL
 */
void fs_llist_remove_next(fs_llist_t* list);

/**
 * Appends list `b` to list `a`
 *  (a)->(b)->NIL
 *  (c)->(d)->NIL
 *     append(a,b)
 * (a)->(b)->(c)->(d)->NIL
 */
void fs_llist_append(fs_llist_t* a, fs_llist_t* b);

#endif
