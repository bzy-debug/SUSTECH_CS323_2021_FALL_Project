#include<stdio.h>
#include<string.h>
#include"syntax.tab.c"
#include"llist.h"
#include"node.h"
#include"type.h"

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
    freopen(outa, "w", stdout);
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    symbol_table = create_llist();

    generate_grammar_tree(f);
    fclose(f);

    if(iserror == 0){
        // print_tree(root, 0);
    }
    else
        return 1;

    llist_node* cur = symbol_table->head->next;
    while (cur != symbol_table->tail)
    {
        MyType* temp = (MyType*) cur->value;
        printf("%s %d ", cur->key, temp->category);
        cur=cur->next;
    }


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
        if(pare->isempty || pare->children == NULL)   continue;

        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"Def") == 0 ) {
            print_tree(pare, 0);

        }
        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    } 
}

MyType* get_primitive_type(node* specifier) {
    node* child = (node*) specifier->children->head->next->value;
    if(child->node_type == eTYPE) {             //primitive type
        return createType(child->val.typeval);
    }
    return NULL;
}

llist_node* get_struct_type(node* struct_specifier) {
    if(struct_specifier->children->size != 5)
        return NULL;
    llist_node* struct_type = create_node(NULL, NULL);
}

llist* get_symbol_table(node* def) {
    llist* temp_table = create_llist(strcpy);
    llist* temp_stack = create_llist(NULL);
    llist_append(temp_stack, create_node(NULL, def));

    MyType* temp_type = get_primitive_type((node*)def->children->head->next->value);

    while (temp_stack->size >= 1)
    {
        node* pare = (node*) llist_pop(temp_stack)->value;
        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(temp_stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    }
    
}