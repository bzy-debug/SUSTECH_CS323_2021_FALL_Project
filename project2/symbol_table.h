#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include"llist.h"
#include"llist_node.h"
#include"type.h"
#include"node.h"

void print_symbol_table(llist* symbol_table);

MyType* get_type_from_specifier(node* specifier);

MyType* get_struct_type(node* struct_specifier);

llist_node* get_symbol_node_from_vardec(node* vardec, MyType* sepecifier_type, MyType* array_type);

llist* get_symbol_node_list_from_def(node* def);

llist* get_symbol_node_list_from_extdef(node* extdef);

llist_node* get_symbol_node_from_fundec(node* fundec, MyType* return_type);

llist_node* get_symbol_node_from_paramdec(node* paramdec);
#endif