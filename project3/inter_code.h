#ifndef __INTER_CODE_H
#define __INTER_CODE_H
#include "llist.h"

typedef struct operand_ operand;
typedef struct inter_code_ inter_code;
typedef enum operand_kind_ op_kind;
typedef enum code_kind_ ic_kind;
typedef enum relop_ relop;

enum operand_kind_ { VARIABLE, CONSTANT, LABEL, oFUNCTION } ;

enum code_kind_{ 
    DEF_LAB, DEF_FUNC, cASSIGN, cADD, cSUB, cMUL, cDIV,
    ADDR, GOTO, cIF, cRETURN, DEC, ARG, CALL, PARAM,
    cREAD, cWRITE, LEFT_S, RIGHT_S , code_list
};

enum relop_ {
    rEQ, rNE, rLT, rGT, rLE, rGE
};

struct operand_ {
    op_kind kind;
    union 
    {
        char* var_name;
        int value;
        int label_no;
        char* func_name;
    } u;
};

struct inter_code_ {
    ic_kind kind;
    union {
        struct { operand* op; } one_op;
        struct { operand *right, *left; } assign;
        struct { operand *result, *op1, *op2; } binop;
        struct { operand *result, *op1, *op2; relop rop;} ifgo;
        llist* code_list;
    } u;    
};

operand* cnt_op_int(op_kind kind, int op_val);

operand* cnt_op_str(op_kind kind, char* op_val);

inter_code* cnt_ic(ic_kind kind, int num, ...);

void print_code(inter_code* code);

void print_op(operand* op);

char* relop_to_string(relop rop);

char* op_to_string(operand* op);

char* code_to_string(inter_code* code);

#endif
