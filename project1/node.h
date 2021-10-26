typedef enum NODE_TYPE {
    eID, eTYPE, eINT, eFLOAT, eCHAR, nterm,
    eSTRUCT, eIF, eELSE, eWHILE, eRETURN,
    eDOT, eSEMI, eCOMMA, eASSIGN,
    eLT, eLE, eGT, eGE, eNE, eEQ, ePLUS, eMINUS, eMUL, eDIV,
    eAND, eOR, eNOT, eLP, eRP, eLB, eRB, eLC, eRC
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
    int isempty; // 0 not empty, 1 empty. for nterms
    int line;
    nodeVal val;
    struct NODE* child;
    struct NODE* sibling;
} node;

void addchild(node* ,int , ... );

void print_tree(node*, int);
