#include"inter_code.h"
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include"llist.h"

operand* cnt_op_int(op_kind kind, int op_val) {
    operand* new_op = malloc(sizeof(operand));
    new_op->kind = kind;
    new_op->u.value = op_val;
    return new_op;
}

operand* cnt_op_str(op_kind kind, char* op_val) {
    operand* new_op = malloc(sizeof(operand));
    new_op->kind = kind;
    new_op->u.func_name = op_val;
    return new_op;
}

inter_code* cnt_ic(ic_kind kind, int num, ...) {
    inter_code* new_code = malloc(sizeof(inter_code));
    new_code->prev = new_code->next = NULL;
    new_code->kind = kind;
    va_list valist;
    va_start(valist, num);
    switch (num)
    {
    case 1:
        new_code->u.one_op.op = va_arg(valist, operand*);
        break;
    case 2:
        new_code->u.assign.left = va_arg(valist, operand*); 
        new_code->u.assign.right = va_arg(valist, operand*);
        break;
    case 3:
        new_code->u.binop.op1 = va_arg(valist, operand*);
        new_code->u.binop.op2 = va_arg(valist, operand*);
        new_code->u.binop.result = va_arg(valist, operand*);
        break; 
    case 4:
        new_code->u.ifgo.op1 = va_arg(valist, operand*);
        new_code->u.ifgo.rop = va_arg(valist, relop);
        new_code->u.ifgo.op2 = va_arg(valist, operand*);
        new_code->u.ifgo.result = va_arg(valist, operand*);
        break;
    default:
        va_end(valist);
        return new_code;
    }
    va_end(valist);
    return new_code;
}

void print_code(inter_code* code) {
    printf("%s\n", code_to_string(code));
}

void print_op(operand* op) {
    printf("%s\n", op_to_string(op));
}

char* relop_to_string(relop rop) {
    char* rop_str = malloc(sizeof(char) * 3);
    switch (rop)
    {
    case rEQ:
        strcpy(rop_str, "==");
        break;
    case rNE:
        strcpy(rop_str, "!=");
        break;
    case rLT:
        strcpy(rop_str, "<");
        break;
    case rGT:
        strcpy(rop_str, ">");
        break;
    case rLE:
        strcpy(rop_str, "<=");
        break;
    case rGE:
        strcpy(rop_str, ">=");
        break;
    default:
        return NULL;
        break;
    }
    return rop_str;
}

char* op_to_string(operand* op) {
    char* op_str = malloc(sizeof(char) * 25);
    switch (op->kind) {
        case VARIABLE:
            op_str = op->u.var_name;
            break;
        case CONSTANT:
            sprintf(op_str, "#%d", op->u.value);
            break;
        case LABEL:
            sprintf(op_str, "label%d", op->u.value);
            break;
        case oFUNCTION:
            op_str = op->u.func_name;
            break;
        default:
            return NULL;
    }
    return op_str;
}

char* code_to_string(inter_code* code) {
    char* code_str = malloc(sizeof(char) * 50);
    operand* op = code->u.one_op.op;
    operand* right = code->u.assign.right;
    operand* left = code->u.assign.left;
    operand* bresult = code->u.binop.result;
    operand* bop1 = code->u.binop.op1;
    operand* bop2 = code->u.binop.op2;
    operand* iresult = code->u.ifgo.result;
    operand* iop1 = code->u.ifgo.op1;
    operand* iop2 = code->u.ifgo.op2;
    relop rop = code->u.ifgo.rop;
    switch (code->kind) {
        case DEF_LAB:
            sprintf(code_str, "LABEL %s :", op_to_string(op));
            break;
        case DEF_FUNC:
            sprintf(code_str, "FUNCTION %s :", op_to_string(op));
            break;
        case GOTO:
            sprintf(code_str, "GOTO %s", op_to_string(op));
            break;
        case cRETURN:
            sprintf(code_str, "RETURN %s", op_to_string(op));
            break;
        case ARG:
            sprintf(code_str, "ARG %s", op_to_string(op));
            break;
        case PARAM:
            sprintf(code_str, "PARAM %s", op_to_string(op));
            break;
        case cREAD:
            sprintf(code_str, "READ %s", op_to_string(op));
            break;
        case cWRITE:
            sprintf(code_str, "WRITE %s", op_to_string(op));
            break;
        case cASSIGN:
            sprintf(code_str, "%s := %s", op_to_string(left), op_to_string(right));
            break;
        case ADDR:
            sprintf(code_str, "%s := &%s", op_to_string(left), op_to_string(right));
            break;
        case LEFT_S:
            sprintf(code_str, "*%s := %s", op_to_string(left), op_to_string(right));
            break;
        case RIGHT_S:
            sprintf(code_str, "%s := *%s", op_to_string(left), op_to_string(right));
            break;
        case DEC:
            sprintf(code_str, "DEC %s %s", op_to_string(left), op_to_string(right));
            break;
        case CALL:
            sprintf(code_str, "%s := CALL %s", op_to_string(left), op_to_string(right));
            break;
        case cADD:
            sprintf(code_str, "%s := %s + %s", op_to_string(bresult), op_to_string(bop1), op_to_string(bop2));
            break;
        case cSUB:
            sprintf(code_str, "%s := %s - %s", op_to_string(bresult), op_to_string(bop1), op_to_string(bop2));
            break;
        case cMUL:
            sprintf(code_str, "%s := %s * %s", op_to_string(bresult), op_to_string(bop1), op_to_string(bop2));
            break;
        case cDIV:
            sprintf(code_str, "%s := %s / %s", op_to_string(bresult), op_to_string(bop1), op_to_string(bop2));
            break;
        case cIF:
            sprintf(code_str, "IF %s %s %s GOTO %s",
            op_to_string(iop1), relop_to_string(rop), op_to_string(iop2), op_to_string(iresult));
            break;
        default:
            return NULL;
    }
    return code_str;
}
