#ifndef __LLIST_H
#define __LLIST_H
#include"llist_node.h"
typedef struct _llist llist;

struct _llist {
    int size;
    llist_node* head;
    llist_node* tail;
    int (*compare_key) (const void*, const void*);
    // char* (*to_string) (void *);
    // void (*free_key) (void *);
    // void (*free_value) (void *);
};

llist* create_llist(
    int (*compare_key) (const void*, const void*)
    // char* (*to_string) (void *),
    // void (*free_key) (void *),
    // void (*free_value) (void *)
);

int llist_add_front(llist*, llist_node*);

int llist_append(llist*, llist_node*);

llist_node* llist_pop(llist*);

int llist_concatenate(llist*, llist*);

llist_node* llist_get(llist*, void*);
#endif