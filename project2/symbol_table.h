#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include"llist.h"
#include"llist_node.h"
#include"type.h"
#include"node.h"

void print_symbol_table(llist* symbol_table);

MyType* get_type_from_specifier(node* specifier, llist* symbol_table, llist* symbol_table_stack);

MyType* get_struct_type(node* struct_specifier, llist* symbol_table, llist* symbol_table_stack);

llist_node* get_symbol_node_from_vardec(node* vardec, MyType* sepecifier_type, MyType* array_type, llist* symbol_table, llist* symbol_table_stack);

llist* get_symbol_node_list_from_def(node* def, llist* symbol_table, llist* symbol_table_stack);

llist* get_symbol_node_list_from_extdef(node* extdef, llist* symbol_table, llist* symbol_table_stack);

llist_node* get_symbol_node_from_fundec(node* fundec, MyType* return_type, llist* symbol_table, llist* symbol_table_stack);

llist_node* get_symbol_node_from_paramdec(node* paramdec, llist* symbol_table, llist* symbol_table_stack);

MyType* get_type_by_key(char* key, llist* symbol_table_stack);

int symbol_table_duplicate(llist* symbol_table_stack, char* key);

int symbol_table_contains_func(llist* symbol_table_stack, char* key);

int symbol_table_contains(llist* symbol_table_stack, char * key);

MyType* get_current_function(llist* symbol_table_stack);

MyType* get_exp_type(node* exp, llist* symbol_table_stack);

#endif