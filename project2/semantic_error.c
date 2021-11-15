#include"semantic_error.h"
#include<stdio.h>

void semantic_error (int type, int line, char* info) {
    switch (type)
    {
    case 1:
        printf("Error type %d at line %d: undefined variable: %s\n", type, line, info);
        break;
    case 2:
        printf("Error type %d at line %d: undefined function: %s\n", type, line, info);
        break;
    case 3:
        printf("Error type %d at line %d: redefine variable: %s\n", type, line, info);
        break;
    case 4:
        printf("Error type %d at Line %d: redefine function: %s\n", type, line, info);
        break;
    }
}