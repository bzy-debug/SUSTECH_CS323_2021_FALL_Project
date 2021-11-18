#include"semantic_error.h"
#include<stdio.h>

void semantic_error (int type, int line, char* info) {
    switch (type)
    {
    case 1:
        printf("Error type %d at Line %d: undefined variable: %s\n", type, line, info);
        break;
    case 2:
        printf("Error type %d at Line %d: undefined function: %s\n", type, line, info);
        break;
    case 3:
        printf("Error type %d at Line %d: redefine variable: %s\n", type, line, info);
        break;
    case 4:
        printf("Error type %d at Line %d: redefine function: %s\n", type, line, info);
        break;
    case 5:
        printf("Error type %d at Line %d: unmatching type on both sides of assignment\n", type, line);
        break;
    case 6:
        printf("Error type %d at Line %d: left side in assignment is rvalue\n", type, line);
        break;
    case 7:
        printf("Error type %d at Line %d: unmatching operands\n", type, line);
        break;
    case 8:
        printf("Error type %d at Line %d: incompatiable return type\n", type, line);
        break;
    case 9:
        printf("Error type %d at Line %d: invalid argument for %s\n",type, line, info);
        break;
    case 10:
        printf("Error type %d at Line %d: indexing on non-array variable\n",type, line);
        break;
    case 11:
        printf("Error type %d at Line %d: invoking non-function variable: %s\n", type, line, info);
        break;
    case 12:
        printf("Error type %d at Line %d: indexing by non-integer\n", type, line);
        break;
    case 13:
        printf("Error type %d at Line %d: access members of a non-structure variable\n", type, line);
        break;
    case 14:
        printf("Error type %d at Line %d: access an undefined structure member\n", type, line);
        break;
    case 15:
        printf("Error type %d at Line %d: redefine struct: %s\n", type, line, info);
        break;
    }
}