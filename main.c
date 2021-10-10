
#include"syntax.tab.c"

int main(int argc, char**argv) {
    if (argc <= 1)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    yylineno = 1;
    // yydebug = 1;
    yyrestart(f);
    yyparse();
    if(iserror == 0)
        print_tree(root, 0);
    return 0;
}