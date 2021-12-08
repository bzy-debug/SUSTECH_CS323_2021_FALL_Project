#ifndef __NODE_H
#define __NODE_H
#include"llist.h"
#include"type.h"

typedef enum NODE_TYPE {
    eID, eTYPE, eINT, eFLOAT, eCHAR, nterm,
    eSTRUCT, eIF, eELSE, eWHILE, eRETURN,
    eDOT, eSEMI, eCOMMA, eASSIGN,
    eLT, eLE, eGT, eGE, eNE, eEQ, ePLUS, eMINUS, eMUL, eDIV,
    eAND, eOR, eNOT, eLP, eRP, eLB, eRB, eLC, eRC,
    eREAD, eWRITE
} nodeType;

typedef union NODE_VAL
{
    int intval;
    float floatval;
    char* charval;
    char* typeval;
    char* idval;
    char* ntermval;
}nodeVal;


typedef struct NODE
{
    nodeType node_type;
    int isempty; // 0 not empty, 1 empty. for nterms. default is 0
    int isexplored; // default is 0
    int left_or_right; // 0 is left, 1 is right, default is 1
    int line;
    nodeVal val;
    MyType* type;
    llist* children;
    struct NODE* pare;
} node;


void addchild(node* ,int , ... );

void print_tree(node*, int);

node* create_grammar_node(nodeType nodeType, int line);

#endif