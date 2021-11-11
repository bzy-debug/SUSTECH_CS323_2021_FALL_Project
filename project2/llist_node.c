#include"llist_node.h"
#include<stdlib.h>

llist_node* create_node(void* key, void* value){
    llist_node* node = malloc(sizeof(llist_node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    
    return node;
}

void free_node(llist_node* node, void (*free_key)(void *), void (*free_value)(void *)){
    if(node == NULL) return;
    (*free_key)(node->key);
    (*free_value)(node->value);
    free(node);
    return;
}