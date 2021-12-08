#include"llist.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

llist* create_llist()
{
    llist* llist_ = malloc(sizeof (llist));

    llist_->size = 0;
    llist_->head = create_node(NULL, NULL);
    llist_->tail = create_node(NULL, NULL);

    llist_->head->next = llist_->tail;
    llist_->tail->prev = llist_->head;

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

llist_node* llist_peak(llist* llist) {
    if(llist->size == 0) {
        return NULL;
    }
    return llist->tail->prev;
}

int llist_concatenate(llist* left, llist* right) {
    left->size += right->size;
    left->tail->prev->next = right->head->next;
    right->head->next->prev = left->tail->prev;
    left->tail = right->tail;
}

llist_node* llist_get_by_key(llist* llist, char* key) {
    llist_node* cur = llist->tail->prev;
    while (cur != llist->head && strcmp(cur->key, key) != 0){
        cur = cur->prev;
    }
    if(cur == llist->head)
        return NULL;
    return cur;
}

int llist_duplicate(llist* llist, char* key) {
    int count = 0;
    llist_node* cur = llist->tail->prev;
    while (cur != llist->head){
        if(strcmp(cur->key, key) == 0)
            count ++;
        cur = cur->prev;
    }
    return count;
}

llist_node* llist_get_by_index(llist* llist, int index) {
    if(index >= llist->size) {
        return NULL;
    }
    llist_node* cur = llist->head->next;
    for(int i=0; i<index; i++) {
        cur = cur->next;
    }
    return cur;
}

int llist_update(llist* llist, char* key, void* new_value) {
    llist_get_by_key(llist, key)->value = new_value;
    return 0;
}

int llist_contains(llist* llist, char* key) {
    void* value = llist_get_by_key(llist, key);
    return value != NULL; 
}


void llist_print(const llist* llist) {
    llist_node* cur = llist->head->next;
    // if(cur == NULL) return;
    while (cur != llist->tail)
    {
        // if(cur == NULL) return;
        printf("%s ", cur->key);
        cur = cur->next;
    }
}

int llist_remove_by_key(llist* llist, char* key) {
    if(llist_contains(llist, key)) {
        llist_node* removed = llist_get_by_key(llist, key);
        removed->prev->next = removed->next;
        removed->next->prev = removed->prev;
        return 0;
    }
    return 1;
}