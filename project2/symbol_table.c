#include"symbol_table.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void print_symbol_table(llist* symbol_table) {
    if(symbol_table == NULL) {
        perror("symbol table is null");
        return;
    }
    llist_node* cur = symbol_table->head->next;
    while (cur != symbol_table->tail)
    {
        MyType* temp = (MyType*) cur->value;
        printf("%s %d ", cur->key, temp->category);
        cur=cur->next;
    }
    printf("\n");
}

MyType* get_type_from_specifier(node* specifier, llist* symbol_table){
    node* child = (node*)specifier->children->head->next->value;
    if(child->node_type == eTYPE) {
        return createType(child->val.typeval);
    }
    else{
        return get_struct_type(child, symbol_table);
    }
}

MyType* get_struct_type(node* struct_specifier, llist* symbol_table){
    if(struct_specifier->children->size == 2) {

    }

    char* struct_id = ((node*)struct_specifier->children->head->next->next->value)->val.idval;
    size_t struct_id_len = strlen(struct_id);
    char* struct_symbol_id = malloc((7+struct_id_len+1) * sizeof(char));
    struct_symbol_id[0] = 's'; struct_symbol_id[1] = 't'; struct_symbol_id[2] = 'r'; struct_symbol_id[3] = 'u';
    struct_symbol_id[4] = 'c'; struct_symbol_id[5] = 't'; struct_symbol_id[6] = ' ';
    for(size_t i=0; i<struct_id_len; i++) {
        struct_symbol_id[7+i] = struct_id[i];
    }

    MyType* struct_type = createType("struct");
    llist* filed_list = create_llist();

    llist* stack = create_llist();
    llist_append(stack, create_node(NULL, struct_specifier));
    while (stack->size >= 1) {
        node* pare = (node*)(llist_pop(stack)->value);
        if(pare->isempty || pare->children == NULL)   continue;

        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"Def") == 0 ) {
            llist_concatenate(filed_list, get_symbol_node_list_from_def(pare, symbol_table));
            continue;
        }

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    }
    llist_node* cur = filed_list->head->next;
    while (cur != filed_list->tail)
    {
        addStructField(struct_type, cur->value, cur->key);
        cur = cur->next;
    }
    llist_append(symbol_table, create_node(struct_symbol_id, struct_type));
    return struct_type;
}

llist_node* get_symbol_node_from_vardec(node* vardec, MyType* sepecifier_type, MyType* array_type, llist* symbol_table) {
    if(vardec->children->size == 1) {
        node* child = (node*) vardec->children->head->next->value;
        if(array_type == NULL)
            return create_node(child->val.idval, sepecifier_type);
        else
            return create_node(child->val.idval, array_type);
        
    }
    else {
        MyType* t = createType("array");
        int size = ((node*)vardec->children->head->next->next->next->value)->val.intval;
        node* next_vardec = (node*)vardec->children->head->next->value;
        setArraySize(t, size);

        if(array_type == NULL) 
            setArrayType(t, sepecifier_type);
        else 
            setArrayType(t, array_type);

        get_symbol_node_from_vardec(next_vardec, sepecifier_type, t, symbol_table);
    }
}

llist* get_symbol_node_list_from_def(node* def, llist* symbol_table) {
    def->isexplored = 1;
    llist* symbol_node_list = create_llist();
    MyType* specifier_type = get_type_from_specifier(def->children->head->next->value, symbol_table);

    llist* stack = create_llist();
    llist_append(stack, create_node(NULL, def->children->head->next->next->value));
    while (stack->size >= 1) {
        node* pare = (node*)(llist_pop(stack)->value);
        if(pare->isempty || pare->children == NULL)   continue;

        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"VarDec") == 0 ) {
            llist_node* t = get_symbol_node_from_vardec(pare, specifier_type, NULL, symbol_table);
            llist_append(symbol_node_list, t);
            continue;
        }

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    }
    return symbol_node_list;
}

llist* get_symbol_node_list_from_extdef(node* extdef, llist* symbol_table) {
    extdef->isexplored = 1;
    if(extdef->children->size == 2) {
        return create_llist();
    }
    else if (((node*)extdef->children->head->next->next->next->value)->node_type == eSEMI) {
        return get_symbol_node_list_from_def(extdef, symbol_table);
    }
    else {  //function type
        llist* func_list = create_llist();
        MyType* return_type = get_type_from_specifier(extdef->children->head->next->value, symbol_table);
        node* fundec = (node*) extdef->children->head->next->next->value;
        llist_node* temp = get_symbol_node_from_fundec(fundec, return_type, symbol_table);
        llist_append(func_list, temp);
        return func_list;
    }
}

llist_node* get_symbol_node_from_fundec(node* fundec, MyType* return_type, llist* symbol_table) {
    char* id = ((node*) fundec->children->head->next->value)->val.idval;
    MyType* func = createType("func");
    setFuncReturnType(func, return_type);
    llist* para_list = create_llist();

    llist* stack = create_llist();
    llist_append(stack, create_node(NULL, fundec));
    while (stack->size >= 1) {
        node* pare = (node*)(llist_pop(stack)->value);
        if(pare->isempty || pare->children == NULL)   continue;

        if(pare->node_type == nterm && strcmp(pare->val.ntermval,"ParamDec") == 0 ) {
            llist_append(para_list, get_symbol_node_from_paramdec(pare, symbol_table));
            continue;
        }

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    }

    llist_node* cur = para_list->head->next;
    while (cur != para_list->tail)
    {
        addFuncParameter(func, cur->value, cur->key);
        cur = cur->next;
    }
    return create_node(id, func);
}

llist_node* get_symbol_node_from_paramdec(node* paramdec, llist* symbol_table) {
    MyType* specifier_type = get_type_from_specifier(paramdec->children->head->next->value, symbol_table);
    return get_symbol_node_from_vardec(paramdec->children->head->next->next->value, specifier_type, NULL, symbol_table);
}