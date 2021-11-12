#include"llist.h"
#include<stdlib.h>

llist* create_llist(
    int (*compare_key) (const void*, const void*)
    // char* (*to_string) (void *),
    // void (*free_key) (void *),
    // void (*free_value) (void *)
    )
{
    llist* llist_ = malloc(sizeof (llist));

    llist_->size = 0;
    llist_->head = create_node(NULL, NULL);
    llist_->tail = create_node(NULL, NULL);

    llist_->head->next = llist_->tail;
    llist_->tail->prev = llist_->head;

    llist_->compare_key = compare_key;
    // llist->to_string = to_string;
    // llist->free_key = free_key;
    // llist->free_value = free_value;
    
    return llist_;
}

int llist_add_front(llist* llist, llist_node* node){

    llist->size += 1;

    node->next = llist->head->next;
    node->prev = llist->head;
    node->next->prev = node;
    llist->head->next = node;

    return 0;
}

int llist_append(llist* llist, llist_node* node) {
    llist->size += 1;
    node->next = llist->tail;
    node->prev = llist->tail->prev;
    llist->tail->prev = node;
    node->prev->next = node;
    return 0;
}

llist_node* llist_pop(llist* llist) {
    if(llist->size == 0) {
        return NULL;
    }
    
    llist->size -= 1;

    llist_node* poping = llist->tail->prev;
    llist->tail->prev = poping->prev;
    poping->prev->next = llist->tail;

    return poping;
}

int llist_concatenate(llist* left, llist* right) {
    left->size += right->size;
    left->tail->prev->next = right->head->next;
    right->head->next->prev = left->tail->prev;
    left->tail = right->tail;
    right->head = left->head;
}

llist_node* llist_get(llist* llist, void* key) {
    llist_node* cur = llist->head->next;
    while (cur != NULL && (llist->compare_key)(cur->key, key) != 0){
        cur = cur->next;
    }
    return cur; 
}
