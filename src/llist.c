#include "fssim/llist.h"

fs_llist_t* fs_llist_create(void* data)
{
  fs_llist_t* list = malloc(sizeof(*list));
  PASSERT(list, FS_ERR_MALLOC);

  list->next = NULL;
  list->data = data;

  return list;
}

void fs_llist_destroy(fs_llist_t* list, fs_llist_destructor destructor)
{
  fs_llist_t* next;

  while (list) {
    next = list->next;

    if (list->data && destructor)
      destructor(list->data);

    free(list);
    list = next;
  }
}

void fs_llist_append(fs_llist_t* a, fs_llist_t* b)
{
  fs_llist_t* tmp = a;

  while ((a = tmp->next) != NULL)
    tmp = a;

  tmp->next = b;
}

void fs_llist_remove(fs_llist_t* list) { list->next = NULL; }
