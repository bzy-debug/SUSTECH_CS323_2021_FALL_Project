#include<stdio.h>
#include<string.h>
#include"syntax.tab.c"
#include"llist.h"
#include"node.h"
#include"type.h"

void generate_grammar_tree(FILE *);
void semantic_check(node* grammar_tree, llist* symbol_table);

int main(int argc, char**argv) {
    if (argc <= 1)
        return 1;
    int l = strlen(argv[1]);
    char* outa = malloc(sizeof(char)*(l+1));
    strcpy(outa, argv[1]);
    outa[l-3]='o'; outa[l-2]='u'; outa[l-1]='t';
    freopen(outa, "w", stdout);
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    generate_grammar_tree(f);
    fclose(f);

    symbol_table = create_llist(NULL);

    if(iserror == 0)
        print_tree(root, 0);
        
    // semantic_check(root, symbol_table);

    return 0;
}

void generate_grammar_tree(FILE* f) {
    yylineno = 1;
    // yydebug = 1;
    yyrestart(f);
    yyparse();
}

void semantic_check(node* grammar_tree, llist* symbol_table) {
    llist* stack = create_llist(NULL);
    llist_append(stack, create_node(NULL, grammar_tree));
    while (stack->size >= 1)
    {
        node* pare = (node*)(llist_pop(stack)->value);
        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"Def") == 0 ) {

        }
        llist_node* cur = pare->children->head->next;
        while (cur)
        {
            llist_append(stack, cur);
            cur = cur->next;
        }
    } 
}