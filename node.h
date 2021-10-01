typedef struct NODE
{
    char* info;
    struct NODE* child;
    struct NODE* sibling;
} node;

void addchild(node* , node* );
