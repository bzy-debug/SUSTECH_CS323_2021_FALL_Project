#include<stdio.h>
#include"syntax.tab.c"
#include"llist.h"

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
    yylineno = 1;
    symbol_table = create_llist(NULL);
    // yydebug = 1;
    yyrestart(f);
    yyparse();
    if(iserror == 0)
        print_tree(root, 0);
    fclose(f);
    return 0;
}