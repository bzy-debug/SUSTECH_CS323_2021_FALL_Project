#include"node.h"
#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>

void print_node(node* n);

node* create_grammar_node(nodeType nodeType, nodeVal val, int line) {
    node* new_node = malloc(sizeof(node));
    new_node->node_type = nodeType;
    new_node->val = val;
    new_node->line = line;
}

void addchild(node* p,int num, ...){
    p->children = create_llist();
    va_list valist;
    va_start(valist, num);
    for(int i=0; i<num; i++) {
        llist_append(p->children, create_node(NULL, va_arg(valist, node*)));
    }
    va_end(valist);
}

void print_tree(node* root, int d){
    if(root->isempty == 1)
        return;
    for(int i=0; i<d; i++)
        printf("  ");
    print_node(root);
    if(!root->children) return;
    if(root->children->size == 0) return;
    
    llist_node* cur = root->children->head->next;
    while (cur != root->children->tail)
    {
        print_tree(cur->value, d+1);
        cur = cur->next;
    }
}

void print_node(node* n){
    switch (n->node_type)
    {
    case eID:
        printf("ID: %s\n", n->val.idval);
        break;
    case eTYPE:
        printf("TYPE: %s\n", n->val.typeval);
        break;
    case eINT:
        printf("INT: %d\n", n->val.intval);
        break;
    case eFLOAT:
        printf("FLOAT: %g\n", n->val.floatval); //TODO: possible bugs with trailing zeros
        break;
    case eCHAR:
        printf("CHAR: %s\n", n->val.charval);
        break;
    case nterm:
        printf("%s (%d)", n->val.ntermval, n->line);
        printf("\n");
        break;
    case eSTRUCT:
        printf("STRUCT\n");
        break;
    case eIF:
        printf("IF\n");
        break;
    case eELSE:
        printf("ELSE\n");
        break;
    case eWHILE:
        printf("WHILE\n");
        break;
    case eRETURN:
        printf("RETURN\n");
        break;
    case eDOT:
        printf("DOT\n");
        break;
    case eSEMI:
        printf("SEMI\n");
        break;
    case eCOMMA:
        printf("COMMA\n");
        break;
    case eASSIGN:
        printf("ASSIGN\n");
        break;
    case eLT:
        printf("LT\n");
        break;
    case eLE:
        printf("LE\n");
        break;
    case eGT:
        printf("GT\n");
        break;
    case eNE:
        printf("NE\n");
        break;
    case eEQ:
        printf("EQ\n");
        break;
    case ePLUS:
        printf("PLUS\n");
        break;
    case eMINUS:
        printf("MINUS\n");
        break;
    case eMUL:
        printf("MUL\n");
        break;
    case eDIV:
        printf("DIV\n");
        break;
    case eAND:
        printf("AND\n");
        break;
    case eOR:
        printf("OR\n");
        break;
    case eNOT:
        printf("NOT\n");
        break;
    case eLP:
        printf("LP\n");
        break;
    case eRP:
        printf("RP\n");
        break;
    case eLB:
        printf("LB\n");
        break;
    case eRB:
        printf("RB\n");
        break;
    case eLC:
        printf("LC\n");
        break;
    case eRC:
        printf("RC\n");
        break;
    default:
        break;
    }
}