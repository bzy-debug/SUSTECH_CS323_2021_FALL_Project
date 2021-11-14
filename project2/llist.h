#ifndef __LLIST_H
#define __LLIST_H
#include"llist_node.h"
typedef struct _llist llist;

struct _llist {
    int size;
    llist_node* head;
    llist_node* tail;
};

llist* create_llist();

int llist_add_front(llist*, llist_node*);

int llist_append(llist*, llist_node*);

llist_node* llist_pop(llist*);

int llist_concatenate(llist*, llist*);

llist_node* llist_get_by_key(llist*, char* key);

llist_node* llist_get_by_index(llist*, int idex);

int llist_update(llist*, void* key, void* new_value);

void llist_print(const llist* llist);
#endif