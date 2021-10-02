#include"node.h"
#include<stdarg.h>
#include<stdio.h>

void print_node(node*);

void addchild(node* p,int num, ...){
    va_list valist;
    va_start(valist, num);
    node* cur = p->child;
    if (cur){
        while (cur->sibling)
            cur = cur->sibling;
        for(int i=0; i<num; i++){
            cur->sibling = va_arg(valist, node*);
            cur = cur->sibling;
        }
    }
    else{
        p->child = va_arg(valist, node*);
        cur = p->child;
        for(int i=1; i<num; i++){
            cur->sibling = va_arg(valist, node*);
            cur = cur->sibling;
        }
    }
}

void print_tree(node* root, int d){
    if(root->isempty == 1)
        return;
    for(int i=0; i<d; i++)
        printf("  ");
    print_node(root);
    node* cur = root->child;
    if (!cur)   return;
    while (cur)
    {
        print_tree(cur, d+1);
        cur = cur->sibling;
    }
}

void print_node(node* n){
    switch (n->node_type)
    {
    case eID:
        printf("ID: %s\n", n->val);
        break;
    case eTYPE:
        printf("TYPE: %s\n", n->val);
        break;
    case eINT:
        printf("INT: %d\n", n->val);
        break;
    case eFLOAT:
        printf("FLOAT: %f\n", n->val);
        break;
    case eCHAR:
        printf("CHAR: %c\n", n->val);
        break;
    case nterm:
        printf("%s\n", n->val);
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