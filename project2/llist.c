#include"llist.h"
#include<stdlib.h>

llist* create_llist(
    int (*compare_key) (const void*, const void*)
    // char* (*to_string) (void *),
    // void (*free_key) (void *),
    // void (*free_value) (void *)
    )
{
    llist* llist = malloc(sizeof llist);

    llist->size = 0;
    llist->head = create_node(NULL, NULL);
    llist->compare_key = compare_key;
    // llist->to_string = to_string;
    // llist->free_key = free_key;
    // llist->free_value = free_value;
    
    return llist;
}

int llist_add_front(llist* llist, llist_node* node){
    node->next = llist->head->next;
    llist->head->next = node;
    return 0;
}

int llist_concatenate(llist* left, llist* right) {
    llist_node* cur = left->head;
    while (cur->next != NULL)
        cur = cur->next;
    cur->next = right->head->next;
}

llist_node* llist_get(llist* llist, void* key) {
    llist_node* cur = llist->head->next;
    while (cur != NULL && (llist->compare_key)(cur->key, key) != 0){
        cur = cur->next;
    }
    return cur; 
}
