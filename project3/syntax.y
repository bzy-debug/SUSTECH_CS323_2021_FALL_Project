%{
    #include"lex.yy.c"
    #include"node.h"
    #include"llist.h"
    #include"llist_node.h"
    #include"type.h"
    #include"symbol_table.h"
    #include<assert.h>
    void yyerror(const char*);
    node* root;
%}
%union {
    node* type_node;
}
%token<type_node> INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE RETURN READ WRITE
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
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Program";
        addchild($$, 1, $1);
        root = $$;
    }
    ;

ExtDefList:
    ExtDef ExtDefList{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDefList";
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDefList";
        $$->children = NULL;
        $$->isempty = 1;
    }
    ;

ExtDef:
    Specifier ExtDecList SEMI{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDef";
        addchild($$, 3, $1, $2, $3);
    }
    | Specifier SEMI{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDef";
        addchild($$, 2, $1, $2);
    }
    | Specifier FunDec CompSt{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDef";
        addchild($$, 3, $1, $2, $3);
    }
    | error FunDec CompSt { printf("Missing specifier\n"); }
    ;

ExtDecList: 
    VarDec{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ExtDecList";
        addchild($$, 1, $1);
    }
    | VarDec COMMA ExtDecList{
        $$ = create_grammar_node(nterm, @$.first_line);
        addchild($$, 3, $1, $2, $3);
    }
    ;

/* specifier */
Specifier: 
    TYPE {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Specifier";
        addchild($$, 1, $1);
    }
    | StructSpecifier{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Specifier";
        addchild($$, 1, $1);
    }
    ;

StructSpecifier: STRUCT ID LC DefList RC{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "StructSpecifier";
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    | STRUCT ID{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "StructSpecifier";
        addchild($$, 2, $1, $2);
    }
    ;

/* declarator */
VarDec: ID{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "VarDec";
        addchild($$, 1, $1);
    }
    | VarDec LB INT RB{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "VarDec";
        addchild($$, 4, $1, $2, $3, $4);
    }
    | VarDec LB INT error {
        printf("Missing closing bracket ']'\n");
    }
    ;

FunDec: ID LP VarList RP{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "FunDec";
        addchild($$, 4, $1, $2, $3, $4);
    }
    | ID LP RP{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "FunDec";
        addchild($$, 3, $1, $2, $3);
    }
    | ID LP VarList error { printf("Missing closing parenthesis ')'\n"); }
    | ID LP error { printf("Missing closing parenthesis ')'\n"); }
    ;

VarList: ParamDec COMMA VarList{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "VarList";
        addchild($$, 3, $1, $2, $3);
    }
    | ParamDec{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "VarList";
        addchild($$, 1, $1);
    }
    ;

ParamDec: Specifier VarDec{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "ParamDec";
        addchild($$, 2, $1, $2);
    }
    ;

/* Statement */
CompSt: LC DefList StmtList RC{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "CompSt";
        addchild($$, 4, $1, $2, $3, $4);
    }
    ;

StmtList: Stmt StmtList{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "StmtList";
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "StmtList";
        $$->children = NULL;
        $$->isempty = 1;
    }
    ;

Stmt: Exp SEMI{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 2, $1, $2);
    }
    | CompSt{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 1, $1);
    }
    | RETURN Exp SEMI{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 3, $1, $2, $3);
    }
    | RETURN Exp error { printf("Missing semicolon ';'\n"); }
    | IF LP Exp RP Stmt %prec LOWERELSE{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 7, $1, $2, $3, $4, $5, $6, $7);
    }
    | WHILE LP Exp RP Stmt{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Stmt";
        addchild($$, 5, $1, $2, $3, $4, $5);
    }
    ;
/* local definition */

DefList: Def DefList{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "DefList";
        addchild($$, 2, $1, $2);
    }
    | %empty {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "DefList";
        $$->children = NULL;
        $$->isempty = 1;
    }
    ;
Def: Specifier DecList SEMI{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Def";
        addchild($$, 3, $1, $2, $3);
    }
    | Specifier DecList error {printf("Missing semicolon ';'\n");}
    | error DecList SEMI {printf("Missing specifier\n"); }
    ;

DecList: Dec{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "DecList";
        addchild($$, 1, $1);
    }
    | Dec COMMA DecList{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "DecList";
        addchild($$, 3, $1, $2, $3);
    }
    ;

Dec: VarDec{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Dec";
        addchild($$, 1, $1);
    }
    | VarDec ASSIGN Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Dec";
        addchild($$, 3, $1, $2, $3);
    }
    ;

/* Expression */
Exp: Exp ASSIGN Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp AND Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp OR Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp LT Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp LE Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp GT Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp GE Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp NE Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp EQ Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp PLUS Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp MINUS Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp MUL Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp DIV Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | LP Exp RP{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | MINUS Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 2, $1, $2);
    }
    | NOT Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 2, $1, $2);
    }
    | ID LP Args RP{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 4, $1, $2, $3, $4);
    }
    | WRITE LP Exp RP {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 4, $1, $2, $3, $4);
    }
    | ID LP RP{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | READ LP RP {
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 3, $1, $2, $3);
    }
    | ID LP Args error{ printf("Missing closing parenthesis ')'\n");} 
    | ID LP error { printf("Missing closing parenthesis ')'\n"); }
    | Exp LB Exp RB{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        $$->left_or_right = 0;
        addchild($$, 4, $1, $2, $3, $4);
    }
    | Exp LB Exp error {
        printf("Missing closing bracket ']'\n");
    }
    | Exp DOT ID{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        $$->left_or_right = 0;
        addchild($$, 3, $1, $2, $3);
    }
    | ID{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        $$->left_or_right = 0;
        addchild($$, 1, $1);
    }
    | INT{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 1, $1);
    }
    | FLOAT{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 1, $1);
    }
    | CHAR{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Exp";
        addchild($$, 1, $1);
    }
    | INVALID_TOKEN
    | Exp INVALID_TOKEN Exp 
    ;
Args: Exp COMMA Args{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Args";
        addchild($$, 3, $1, $2, $3);
    }
    | Exp{
        $$ = create_grammar_node(nterm, @$.first_line);
        $$->val.ntermval = "Args";
        addchild($$, 1, $1);
    }
    ;
%%

void yyerror(const char* s) {
    iserror = 1;
    printf("Error type B at Line %d: ", yylineno);
}
