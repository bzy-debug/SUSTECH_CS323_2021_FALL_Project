#ifndef __LLIST_NODE_H
#define __LLIST_NODE_H
typedef struct _llist_node llist_node;
struct _llist_node
{
    void* key;
    void* value;
    llist_node* next;
    llist_node* prev;
};

llist_node* create_node(void*, void*);

void free_node(llist_node* node, void (*free_key)(void *), void (*free_value)(void *));
#endif