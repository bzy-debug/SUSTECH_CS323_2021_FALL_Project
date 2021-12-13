# Report

## IR Data Structure
The generated IRs are stored in memory using the data structure defined in `inter_code.h`. It is like this:
```
struct operand {
    enum { VARIABLE, CONSTANT, ... } kind;
    union {
        char* var_name;
        int value;
        ...
    } u;
}
struct inter_code {
    enum { ASSIGN, ADD, SUB, ... } kind;
    union {
        struct { operand right, left; } assign;
        struct { operand result, op1, op2; } binop;
        ...
    } u;
    inter_code* prev;
    inter_code* next;
}
```
It is just the enumeration of all kinds of three address code. In addition, to support the code catenate function, two pointers `prev` and `next` are added to make the inter_code doubly linked list. Therefore, we only need to traverse the linked list when printing the codes.