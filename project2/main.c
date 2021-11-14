#include<stdio.h>
#include<string.h>
#include"syntax.tab.c"
#include"llist.h"
#include"node.h"
#include"type.h"
#include"symbol_table.h"

void generate_grammar_tree(FILE *);
void semantic_check(node* grammar_tree, llist* symbol_table);
MyType* get_type(node* specifier);

int main(int argc, char**argv) {
    if (argc <= 1)
        return 1;
    int l = strlen(argv[1]);
    char* outa = malloc(sizeof(char)*(l+1));
    strcpy(outa, argv[1]);
    outa[l-3]='o'; outa[l-2]='u'; outa[l-1]='t';
    // freopen(outa, "w", stdout);
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    symbol_table = create_llist();

    generate_grammar_tree(f);
    fclose(f);

    if(iserror == 0){
        print_tree(root, 0);
    }
    else
        return 1;

    semantic_check(root, symbol_table);
    print_symbol_table(symbol_table);

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
        if(pare->isempty || pare->children == NULL)   continue;

        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"Def") == 0 ) {
            llist* t = get_symbol_node_list_from_def(pare);
            llist_concatenate(symbol_table, t);
            // print_symbol_table(symbol_table);
            continue;
        }
        else if (pare->node_type == nterm && strcmp(pare->val.ntermval,"ExtDef") == 0) {
            llist* t = get_symbol_node_list_from_extdef(pare);
            llist_concatenate(symbol_table, t);
            // print_symbol_table(symbol_table);
        }

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    } 
}