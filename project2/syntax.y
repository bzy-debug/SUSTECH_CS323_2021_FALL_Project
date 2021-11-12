%{
    #include"lex.yy.c"
    #include"node.h"
    #include"llist.h"
    #include"llist_node.h"
    void yyerror(const char*);
    node* root;
    llist* symbol_table;
%}
%union {
    node* type_node;
}
%token<type_node> INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE RETURN
%token<type_node> INVALID_TOKEN
%token<type_node> DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC
%right ASSIGN
%left OR
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left MUL DIV
%right NOT
%left DOT LP RP LB RB
%nonassoc LOWERELSE
%nonassoc ELSE
%left INVALID_TOKEN //TODO
%type<type_node> Program ExtDefList ExtDef ExtDecList
%type<type_node> Specifier StructSpecifier
%type<type_node> VarDec FunDec VarList ParamDec
%type<type_node> CompSt StmtList Stmt
%type<type_node> DefList Def DecList Dec
%type<type_node> Exp Args
%%
/* high-level definition*/
Program:
    ExtDefList
    {
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Program";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        root = $$;
    }
    ;

ExtDefList:
    ExtDef ExtDefList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDefList";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDefList";
        $$->child = NULL;
        $$->isempty = 1;
    }
    ;

ExtDef:
    Specifier ExtDecList SEMI{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDef";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Specifier SEMI{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDef";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | Specifier FunDec CompSt{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDef";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | error FunDec CompSt { printf("Missing specifier\n"); }
    ;

ExtDecList: 
    VarDec{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDecList";
        $$->line = @$.first_line;
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    | VarDec COMMA ExtDecList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ExtDecList";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    ;

/* specifier */
Specifier: 
    TYPE {
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Specifier";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = create_node(NULL, $1->val.typeval);
    }
    | StructSpecifier{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Specifier";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    ;

StructSpecifier: STRUCT ID LC DefList RC{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "StructSpecifier";
        $$->line = @$.first_line;
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    | STRUCT ID{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "StructSpecifier";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    ;

/* declarator */
VarDec: ID{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "VarDec";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = create_node($1->val.idval, "uncertain");
    }
    | VarDec LB INT RB{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "VarDec";
        $$->line = @$.first_line;
        addchild($$, 4, $1, $2, $3, $4);
        $$->syn_node = create_node($1->syn_node->key, "Array");
    }
    | VarDec LB INT error {
        printf("Missing closing bracket ']'\n");
    }
    ;

FunDec: ID LP VarList RP{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "FunDec";
        $$->line = @$.first_line;
        addchild($$, 4, $1, $2, $3, $4);
    }
    | ID LP RP{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "FunDec";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);

    }
    | ID LP VarList error { printf("Missing closing parenthesis ')'\n"); }
    | ID LP error { printf("Missing closing parenthesis ')'\n"); }
    ;

VarList: ParamDec COMMA VarList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "VarList";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | ParamDec{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "VarList";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    ;

ParamDec: Specifier VarDec{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "ParamDec";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    ;

/* Statement */
CompSt: LC DefList StmtList RC{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "CompSt";
        $$->line = @$.first_line;
        addchild($$, 4, $1, $2, $3, $4);
    }
    ;

StmtList: Stmt StmtList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "StmtList";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "StmtList";
        $$->line = @$.first_line;
        $$->child = NULL;
        $$->isempty = 1;
    }
    ;

Stmt: Exp SEMI{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | CompSt{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    | RETURN Exp SEMI{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | RETURN Exp error { printf("Missing semicolon ';'\n"); }
    | IF LP Exp RP Stmt %prec LOWERELSE{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 7, $1, $2, $3, $4, $5, $6, $7);
    }
    | WHILE LP Exp RP Stmt{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Stmt";
        $$->line = @$.first_line;
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    ;
/* local definition */

DefList: Def DefList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "DefList";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "DefList";
        $$->child = NULL;
        $$->isempty = 1;
    }
    ;
Def: Specifier DecList SEMI{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Def";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Specifier DecList error {printf("Missing semicolon ';'\n");}
    | error DecList SEMI {printf("Missing specifier\n"); }
    ;

DecList: Dec{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "DecList";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_list = create_llist(NULL);
        llist_add_front($$->syn_list, $1->syn_node);
    }
    | Dec COMMA DecList{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "DecList";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
        $$->syn_list = create_llist(NULL);
        llist_add_front($$->syn_list, $1->syn_node);
        llist_concatenate($$->syn_list, $3->syn_list);
    }
    ;

Dec: VarDec{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Dec";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = $1->syn_node;
    }
    | VarDec ASSIGN Exp{            //TODO:type checking
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Dec";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
        $$->syn_node = $1->syn_node;
    }
    ;

/* Expression */
Exp: Exp ASSIGN Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp AND Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp OR Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp LT Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp LE Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp GT Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp GE Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp NE Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp EQ Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp PLUS Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp MINUS Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp MUL Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp DIV Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | LP Exp RP{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | MINUS Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | NOT Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 2, $1, $2);
    }
    | ID LP Args RP{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 4, $1, $2, $3, $4);
    }
    | ID LP RP{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | ID LP Args error{ printf("Missing closing parenthesis ')'\n");} 
    | ID LP error { printf("Missing closing parenthesis ')'\n"); }
    | Exp LB Exp RB{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 4, $1, $2, $3, $4);
    }
    | Exp LB Exp error {
        printf("Missing closing bracket ']'\n");
    }
    | Exp DOT ID{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | ID{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    | INT{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = create_node(NULL, "int");
    }
    | FLOAT{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = create_node(NULL, "float");
    }
    | CHAR{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
        $$->syn_node = create_node(NULL, "char");
    }
    | INVALID_TOKEN
    | Exp INVALID_TOKEN Exp 
    ;
Args: Exp COMMA Args{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 3, $1, $2, $3);
    }
    | Exp{
        $$ = malloc(sizeof(node));
        $$->node_type = nterm;
        $$->val.ntermval = "Exp";
        $$->line = @$.first_line;
        addchild($$, 1, $1);
    }
    ;
%%

void yyerror(const char* s) {
    iserror = 1;
    printf("Error type B at Line %d: ", yylineno);
}