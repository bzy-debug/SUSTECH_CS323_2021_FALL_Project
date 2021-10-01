#include"node.h"

void addchild(node* p, node* c){
    node* cur = p->child;
    if (cur){
        while (cur->sibling)
            cur = cur->sibling;
        cur->sibling = c;
    }
    else{
        p->child = c;
    }
}