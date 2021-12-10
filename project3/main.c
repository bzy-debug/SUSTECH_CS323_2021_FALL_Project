#include<stdio.h>
#include<string.h>
#include"syntax.tab.c"
#include"llist.h"
#include"node.h"
#include"type.h"
#include"symbol_table.h"
#include"semantic_error.h"
#include"inter_code.h"
#include<stdarg.h>

void generate_grammar_tree(FILE *);
void semantic_check(node* grammar_tree, llist* symbol_table);

inter_code*  generate_IR(node* grammar_tree);
inter_code* translate_Exp(node* Exp, char* place);
inter_code* translate_cond_Exp(node* Exp,int  lb1,int  lb2);
inter_code* translate_Stmt(node* Stmt);
inter_code* translate_Arg(node* Arg,llist* arg_list);
inter_code* translate_FunDec(node* Fundec);
inter_code* translate_VarList(node* VarList,inter_code* arg_list);
inter_code* translate_VarDec(node* vardec);
inter_code* translate_Dec(node* dec);
inter_code* translate_StructSpecifier(node* Struct_S);


inter_code* ir_concatenate(int num,...);

char* new_place();
int  new_lable();

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

    generate_grammar_tree(f);
    generate_IR(root);
    fclose(f);

    if(iserror == 0){
        print_tree(root, 0);
    }
    else
        return 1;

    llist* symbol_table_stack = create_llist();     //value: symbol_table
    // semantic_check(root, symbol_table_stack);

    // llist_node* cur = symbol_table_stack->head->next;
    // while (cur != symbol_table_stack->tail)
    // {
    //     print_symbol_table(cur->value);
    //     cur = cur->next;
    // }

    return 0;
}

void generate_grammar_tree(FILE* f) {
    yylineno = 1;
    // yydebug = 1;
    yyrestart(f);
    yyparse();
}

void semantic_check(node* grammar_tree, llist* symbol_table_stack) {
    llist* symbol_table = create_llist();
    llist_append(symbol_table_stack, create_node(NULL, symbol_table));

    llist* stack = create_llist(NULL);
    llist_append(stack, create_node(NULL, grammar_tree));
    while (stack->size >= 1)
    {
        node* pare = (node*)llist_pop(stack)->value;

        if(pare->node_type == eRC && pare->pare->node_type == nterm && strcmp(pare->pare->val.ntermval,"CompSt") == 0) {
            llist_pop(symbol_table_stack);
            symbol_table = llist_peak(symbol_table_stack)->value;
        }

        if(pare->isempty || pare->children == NULL)   continue;

        else if(pare->node_type == nterm && strcmp(pare->val.ntermval,"CompSt") == 0 ) {
            symbol_table = create_llist();
            llist_append(symbol_table_stack, create_node(NULL, symbol_table));

            if(strcmp(pare->pare->val.ntermval, "ExtDef") == 0) {
                node* func_node = (node*)pare->pare->children->head->next->next->value;
                char* func_id = ((node*)func_node->children->head->next->value)->val.idval;

                if(symbol_table_contains_func(symbol_table_stack, func_id)) {
                    semantic_error(4, func_node->line, func_id);
                }

                MyType* func_type = get_type_by_key(func_id, symbol_table_stack);
                llist_concatenate(symbol_table, get_func_parameter(func_type));
            }
        }

        else if(pare->node_type == nterm && strcmp(pare->val.ntermval,"Def") == 0 && pare->isexplored == 0) {
            llist* t = get_symbol_node_list_from_def(pare, symbol_table, symbol_table_stack);
            
            llist_node* cur = t->head->next;
            while (cur != t->tail)
            {
                // redefine 仅限当前symbol_table
                if(llist_contains(symbol_table, cur->key)) {
                    semantic_error(3, pare->line, cur->key);
                }
                cur = cur->next;
            }
            
            llist_concatenate(symbol_table, t);
        }

        else if (pare->node_type == nterm && strcmp(pare->val.ntermval,"ExtDef") == 0 && pare->isexplored == 0) {
            llist* t = get_symbol_node_list_from_extdef(pare, symbol_table, symbol_table_stack);
            llist_concatenate(symbol_table, t);
            // print_symbol_table(symbol_table);
        }

        else if (pare->node_type == nterm && strcmp(pare->val.ntermval, "Exp") == 0 && pare->isexplored == 0) {
            get_exp_type(pare, symbol_table_stack);
        }

        else if (pare->node_type == nterm && strcmp(pare->val.ntermval, "Stmt") == 0 && pare->children->size == 3) {
            MyType* current_function = get_current_function(symbol_table_stack);
            node* exp = (node*) pare->children->head->next->next->value;
            MyType* expected_return_type = current_function->function->returnType;
            MyType* actual_return_type = get_exp_type(exp, symbol_table_stack);
            if(actual_return_type == NULL) {
                semantic_error(8, exp->line, "");
            }
            else if (typeEqual(expected_return_type, actual_return_type) == -1) {
                semantic_error(8, exp->line, "");
            }
        }

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    } 
}

inter_code* generate_IR(node* grammar_tree){

    llist* stack = create_llist(NULL);
    llist_append(stack, create_node(NULL, grammar_tree));
    while (stack->size >= 1)
    {
        node* pare = (node*)llist_pop(stack)->value;
        if (pare->node_type == nterm && strcmp(pare->val.ntermval, "Stmt") == 0) {
             inter_code*  ir = translate_Stmt(pare);
            if (ir != NULL){
                print_code(ir);
            }
            
        }
        else if (pare->node_type == nterm && strcmp(pare->val.ntermval, "FunDec") == 0) {
            translate_FunDec(pare);
        }
        else if (pare->node_type == nterm && strcmp(pare->val.ntermval, "Dec") == 0) {
            print_code(translate_Dec(pare));
        }

        
        if(pare->isempty || pare->children == NULL)   continue;

        llist_node* cur = pare->children->tail->prev;
        while (cur != pare->children->head)
        {
            llist_append(stack, create_node(NULL, cur->value));
            cur = cur->prev;
        }
    } 
}

inter_code* translate_Exp(node* Exp, char* place){

        node* pare = (node*)Exp->children->head->next->value;
        //table 1
        if(pare->node_type == eINT&& Exp->children->size == 1) {
            int value = pare->val.intval;

            return cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE,place), cnt_op_int(CONSTANT, value));
        }
        else if(pare->node_type == eID&& pare->pare->children->size == 1) {
            char* value = pare->val.idval;
            return cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE,place), cnt_op_str(VARIABLE, value));
        }
        else if(pare->node_type ==nterm&& strcmp(pare->val.ntermval, "Exp") == 0 && pare->pare->children->size == 3) {
            node* second_node = (node*)pare->pare->children->head->next->next->value;
            node* third_node =  (node*)pare->pare->children->head->next->next->next->value;
            if(second_node->node_type == eASSIGN){
                 node* exp1_id_node = (node*)pare->children->head->next->value;
                 char*value = exp1_id_node->val.idval;
                 char* tp= new_place();
                 inter_code* code1 = translate_Exp(third_node,tp);
                 inter_code* code2 = cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE,value), cnt_op_str(VARIABLE,tp));
                 inter_code* code3 = cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE,place), cnt_op_str(VARIABLE,value));
                 return ir_concatenate(3,code1,code2,code3);
            }
            else if(second_node->node_type == ePLUS){
                char* t1= new_place();
                char* t2 = new_place();
                inter_code* code1 = translate_Exp(pare,t1);
                inter_code* code2 = translate_Exp(pare,t2);
                inter_code* code3 = cnt_ic(cADD, 3, cnt_op_str(VARIABLE, t1), cnt_op_str(VARIABLE, t2), cnt_op_str(VARIABLE,place));
                return ir_concatenate(3,code1,code2,code3);
            }else if(second_node->node_type == eDOT){
                //Exp DOT ID
                char* t1 = new_place();
                inter_code* code1 = translate_Exp(Exp,t1);
                // char* t2 = new_place();
                // code2 = [tp := struct.name + #(offset(ID)*4)];
                // inter_code* code3 = [place := *tp];
                // return code1 + code2 + code3
            }
        }else if(pare->node_type ==eMINUS && pare->pare->children->size == 2) {
            node* second_node = (node*)pare->pare->children->head->next->next->value;
            if(second_node->node_type ==nterm&& strcmp(second_node->val.ntermval, "Exp") == 0){
                char* tp = new_place();
                inter_code* code1 = translate_Exp(second_node, tp);
                inter_code* code2 = cnt_ic(cSUB, 3, cnt_op_int(CONSTANT, 0), cnt_op_str(VARIABLE, tp), cnt_op_str(VARIABLE,place));
                return ir_concatenate(2,code1,code2);
            }
        }
        else if(pare->node_type ==nterm&& strcmp(pare->val.ntermval, "Exp") == 0 && pare->pare->children->size == 4) {
            //Exp1 LB Exp2 RB
            node* exp2_node = (node*)Exp->children->head->next->next->next->value;

            char* t1 = new_place();
            char* t2 = new_place();
            char* t3 = new_place();
            char* t4 = new_place();

            inter_code* code1 = translate_Exp(exp2_node,t1);
            inter_code* code2 = translate_Exp(pare,t2);
            inter_code* code3 = cnt_ic(cMUL,3,cnt_op_str(VARIABLE,t1),cnt_op_int(CONSTANT,4),cnt_op_str(VARIABLE,t3));

            
            inter_code* code4 = cnt_ic(cADD,3,cnt_op_str(VARIABLE,t2),cnt_op_str(VARIABLE,t3),cnt_op_str(VARIABLE,t4));
            inter_code* code5 = cnt_ic(cASSIGN,2,cnt_op_str(VARIABLE,place),cnt_op_str(VARIABLE,t4));
            return  ir_concatenate( 5,code1 , code2 , code3 , code4 , code5);
        }

        //table 4
        if(pare->node_type == eREAD){
            return cnt_ic(cREAD,1,cnt_op_str(VARIABLE,place));
        }else if(pare->node_type == eWRITE){
            char* tp = new_place();
            //TODO: debug write text not showing
            node* exp_node = (node*)pare->pare->children->head->next->next->next->value;
            return ir_concatenate(2, translate_Exp(exp_node,tp), cnt_ic(cWRITE,1,cnt_op_str(VARIABLE,tp)));
        }else if(pare->node_type == eID&& Exp->children->size == 3){
            char* value = pare->val.idval;
            return cnt_ic(CALL,2,cnt_op_str(VARIABLE,place),cnt_op_str(oFUNCTION,value));
        }else if(pare->node_type == eID&& Exp->children->size == 4){
            node* args_node = (node*)pare->pare->children->head->next->next->next->value;

            char* value = pare->val.idval;
            llist* arg_list = create_llist();
            inter_code* code1 = translate_Arg(args_node, arg_list);
            inter_code* code2 = NULL;

            llist_node* arg_head =  arg_list->head;
            for (int i = 1; i < arg_list->size ; i++){
                code2 = ir_concatenate(2, code2 , cnt_ic(ARG,1 ,cnt_op_str(VARIABLE, (char*)arg_head->value)));
                arg_head = arg_head->next;
            }
            return ir_concatenate(3, code1 , code2 ,cnt_ic(CALL,2,cnt_op_str(VARIABLE, place),cnt_op_str(VARIABLE, value)));
        }
        //condi exp
    int  lb1 = new_lable();
    int  lb2 = new_lable();
    inter_code* code0 = cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE, place), cnt_op_int(CONSTANT,0));
    inter_code* code1 = translate_cond_Exp(pare, lb1, lb2);
    inter_code* code2 = ir_concatenate(2, cnt_ic(DEF_LAB,1, cnt_op_int(LABEL,lb1)), cnt_ic(cASSIGN, 2,cnt_op_str(VARIABLE, place), cnt_op_int(CONSTANT,1)));
    return ir_concatenate(4,code0 , code1 , code2 ,cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb2)) ) ;

}

//if nothing pare return: LABEL label-4:
inter_code* translate_cond_Exp(node* Exp,int  lb_t,int  lb_f){
    node* pare = (node*)Exp->children->head->next->value;
    if(pare->node_type ==nterm&& strcmp(pare->val.ntermval, "Exp") == 0 && pare->pare->children->size == 3) {
            node* second_node = (node*)pare->pare->children->head->next->next->value;
            node* third_node =  (node*)pare->pare->children->head->next->next->next->value;
            
            if(second_node->node_type == eEQ){
                 
                char* t1= new_place();
                char* t2= new_place();

                 inter_code* code1 = translate_Exp(pare,t1);
                 inter_code* code2 = translate_Exp(third_node,t2);
                 inter_code* code3 =ir_concatenate(2, cnt_ic(cIF,4,cnt_op_str(VARIABLE,t1), rEQ ,cnt_op_str(VARIABLE,t2), cnt_op_int(LABEL,lb_t)),  cnt_ic(GOTO,1,cnt_op_int(LABEL,lb_f) ));
                 return ir_concatenate(3,code1,code2,code3);
            }
            else if(second_node->node_type == eGE){
                 
                char* t1= new_place();
                char* t2= new_place();

                 inter_code* code1 = translate_Exp(pare,t1);
                 inter_code* code2 = translate_Exp(third_node,t2);
                 inter_code* code3 =ir_concatenate(2, cnt_ic(cIF,4,cnt_op_str(VARIABLE,t1), rGE ,cnt_op_str(VARIABLE,t2), cnt_op_int(LABEL,lb_t)),  cnt_ic(GOTO,1,cnt_op_int(LABEL,lb_f) ));
                 return ir_concatenate(3,code1,code2,code3);
            }else if(second_node->node_type == eLE){
                 
                char* t1= new_place();
                char* t2= new_place();

                 inter_code* code1 = translate_Exp(pare,t1);
                 inter_code* code2 = translate_Exp(third_node,t2);
                 inter_code* code3 =ir_concatenate(2, cnt_ic(cIF,4,cnt_op_str(VARIABLE,t1), eLE ,cnt_op_str(VARIABLE,t2), cnt_op_int(LABEL,lb_t)),  cnt_ic(GOTO,1,cnt_op_int(LABEL,lb_f) ));
                 return ir_concatenate(3,code1,code2,code3);
            }

            else if(second_node->node_type == eAND){
            int  lb1= new_lable();
                inter_code* code1 = ir_concatenate(2, translate_cond_Exp(pare,lb1,lb_f), cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb1)));
                inter_code* code2 = translate_cond_Exp(pare,lb_t,lb_f);
                return ir_concatenate(2,code1,code2);
            } 
            else if(second_node->node_type == eOR){
            int  lb1= new_lable();
                inter_code* code1 = ir_concatenate(2, translate_cond_Exp(pare,lb_t,lb1), cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb1)));
                inter_code* code2 = translate_cond_Exp(pare,lb_t,lb_f);
                return ir_concatenate(2,code1,code2);
            }


    }else if(pare->node_type == eNOT) {
        return translate_cond_Exp(Exp,lb_f,lb_t);
    }

    return cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,-4));
}

//if nothing pare return: LABEL label-3:
inter_code* translate_Stmt(node* Stmt){
    node* pare = (node*)Stmt->children->head->next->value;
    if(pare->node_type ==nterm && strcmp(pare->val.ntermval, "Exp") == 0){
        char* tp = new_place();
        return translate_Exp(pare,tp);
    }
    else 
    if(pare->node_type ==eRETURN){
        node* second_node = (node*)pare->pare->children->head->next->next->value;

        char* tp = new_place();
        inter_code* code = translate_Exp(second_node, tp);
        return ir_concatenate(2, code , cnt_ic(cRETURN,1 ,cnt_op_str(VARIABLE,tp)));

    }else if(pare->node_type ==eIF&& pare->pare->children->size <= 6) {
            node* exp_node = (node*)pare->pare->children->head->next->next->next->value;
            node* stmt_node =  (node*)pare->pare->children->head->next->next->next->next->next->value;

        int  lb1 = new_lable();
        int  lb2 = new_lable();
            inter_code* code1 = ir_concatenate(2, translate_cond_Exp(exp_node, lb1, lb2) , cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb1)));
            inter_code* code2 = ir_concatenate(2, translate_cond_Exp(stmt_node, lb1, lb2) , cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb2)));
            return ir_concatenate(2, code1 , code2);

    }else if(pare->node_type ==eIF&& pare->pare->children->size >= 6) {
            node* exp_node = (node*)pare->pare->children->head->next->next->next->value;
            node* stmt_1_node =  (node*)pare->pare->children->head->next->next->next->next->next->value;
            node* stmt_2_node =  (node*)pare->pare->children->head->next->next->next->next->next->next->next->value;

        int  lb1 = new_lable();
        int  lb2 = new_lable();
        int  lb3 = new_lable();

            inter_code* code1 = ir_concatenate(2, translate_cond_Exp(exp_node, lb1, lb2) , cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb1)));
            inter_code* code2 = ir_concatenate(3, translate_Stmt(stmt_1_node) ,cnt_ic(GOTO,1,cnt_op_int(LABEL,lb3)) , cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb2)));
            inter_code* code3 = ir_concatenate(2, translate_Stmt(stmt_2_node) , cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb3)));

            return ir_concatenate(3, code1 , code2,code3);
    }else if(pare->node_type ==eWHILE) {
            node* exp_node = (node*)pare->pare->children->head->next->next->next->value;
            node* stmt_node =  (node*)pare->pare->children->head->next->next->next->next->next->value;

        int lb1 = new_lable();
        int  lb2 = new_lable();
        int  lb3 = new_lable();

            inter_code* code1 = ir_concatenate(2,cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb1)), translate_cond_Exp(exp_node, lb2, lb3));
            inter_code* code2 = ir_concatenate(3,cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb2)), translate_Stmt(exp_node),cnt_ic(GOTO,1,cnt_op_int(LABEL,lb1)));

            return ir_concatenate(3, code1 , code2,cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,lb3)));
    }

    return cnt_ic(DEF_LAB,1,cnt_op_int(LABEL,-3)); 
}

inter_code* translate_Arg(node* Arg,llist* arg_list){
    node* exp_node = (node*)Arg->children->head->next->value;
    if(Arg->children->size == 1){
        char* tp = new_place();
        inter_code* code = translate_Exp(exp_node, tp);
        llist_add_front(arg_list,create_node(tp,tp));
        return code;
    }else{
        node* args_node = (node*)Arg->children->head->next->next->next->value;
        char* tp = new_place();
        inter_code* code1 = translate_Exp(exp_node, tp);
        llist_add_front(arg_list,create_node(tp,tp));
        inter_code* code2 = translate_Arg(args_node, arg_list);
        return ir_concatenate(2,code1 ,code2);
    }

}

inter_code* translate_FunDec(node* Fundec){
    node* pare = (node*)Fundec->children->head->next->value;

    if(Fundec->children->size == 3) {
        char* value = pare->val.idval;
        return cnt_ic( DEF_FUNC,1,cnt_op_str(oFUNCTION,value));
    }else{
        char* value = pare->val.idval;
       inter_code* code1 =  cnt_ic( DEF_FUNC,1,cnt_op_str(oFUNCTION,value));

        node* varlist_node = (node*)Fundec->children->head->next->next->next->value;
       inter_code* code2 = NULL;
       translate_VarList(varlist_node,code2);
       return ir_concatenate(2,code1,code2);
    }

}

inter_code* translate_VarList(node* VarList,inter_code* param_code){
     node* param_node = (node*)VarList->children->head->next->value;
     node* var_node = (node*)param_node->children->head->next->next->value;
     node* id_node = (node*)var_node->children->head->next->value;

    char* value = id_node->val.idval;
    param_code = ir_concatenate(2,cnt_ic(PARAM,1,cnt_op_str(VARIABLE,value)),param_code);

    if(VarList->children->size == 3) {
        node* varlist_node = (node*)VarList->children->head->next->next->next->value;
        translate_VarList(varlist_node,param_code);
    }
    return param_code;
}
    
inter_code* translate_Dec(node* dec){
    node* vardec_node= (node*)dec->children->head->next->value;
    if (dec->children->size == 1){
        return translate_VarDec(vardec_node);
    }
    // else{
    //     char* tp = new_place();
    //     node* exp_node= (node*)dec->children->head->next->next->next->value;
    //     inter_code* code1 = translate_Exp(exp_node,tp);

    // }
}


inter_code* translate_VarDec(node* vardec_node){
     int size = 4;
     while (vardec_node->children->size == 4){
         node* varec2_node= (node*)vardec_node->children->head->next->value;
         node* int_node = (node*)vardec_node->children->head->next->next->next->value;
         size *= int_node->val.intval;
         vardec_node = varec2_node;
     }
     node* id_node= (node*)vardec_node->children->head->next->value;
     char* name = id_node->val.idval;
     return cnt_ic(DEC,2,cnt_op_str(VARIABLE,name),cnt_op_int(CONSTANT,size));
}

inter_code* translate_StructSpecifier(node* Struct_S){
    if(Struct_S->children->size == 2) {
        node* Struct_node = (node*)Struct_S->children->head->next->value;
        node* ID_node = (node*)Struct_S->children->head->next->next->value;
        
        char* tp = new_place();
        int size = 4;
    }

}


char* new_place(){
    //TODO
    return "t3";
}
int  new_lable(){
    //TODO
    return 4;
}

inter_code* ir_concatenate(int num,...){
//TODO
    va_list codes;
    va_start(codes,num);
    while (num >1)
    {
        inter_code* code = va_arg(codes,inter_code*);
        if (code!=NULL){
            print_code(code);
        }
        num--;

    }
    va_end(codes);
    return NULL;

}
