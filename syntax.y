%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}
%token INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE RETURN
%token INVALID_TOKEN INVALID_ID
%token DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC
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
%%
/* high-level definition*/
Program:
    ExtDefList
    ;

ExtDefList:
    ExtDef ExtDefList
    | %empty
    ;

ExtDef:
    Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;

ExtDecList: VarDec
    | VarDec COMMA ExtDecList
    ;

/* specifier */
Specifier: TYPE
    | StructSpecifier
    ;

StructSpecifier: STRUCT ID LC DefList RC
    | STRUCT ID
    | STRUCT INVALID_ID LC DefList RC
    | STRUCT INVALID_ID
    ;

/* declarator */
VarDec: ID
    | VarDec LB INT RB
    | INVALID_ID
    ;

FunDec: ID LP VarList RP
    | ID LP RP
    | ID LP VarList error { printf("missing closing parenthesis\n"); }
    | ID LP error { printf("missing closing parenthesis\n");}
    | INVALID_ID LP VarList RP
    | INVALID_ID LP RP
    ;

VarList: ParamDec COMMA VarList
    | ParamDec
    ;

ParamDec: Specifier VarDec
    ;

/* Statement */
CompSt: LC DefList StmtList RC
    ;

StmtList: Stmt StmtList
    | %empty
    ;

Stmt: Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | RETURN Exp error { printf("missing colon\n"); }
    | IF LP Exp RP Stmt %prec LOWERELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;
/* local definition */

DefList: Def DefList
    | %empty
    ;
Def: Specifier DecList SEMI
    | Specifier DecList error {printf("missing colon\n"); }
    | error DecList SEMI {printf("missing specifier\n"); }
    ;

DecList: Dec
    | Dec COMMA DecList
    ;

Dec: VarDec
    | VarDec ASSIGN Exp
    ;

/* Expression */
Exp: Exp ASSIGN Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp LT Exp
    | Exp GT Exp
    | Exp GE Exp
    | Exp NE Exp
    | Exp EQ Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp MUL Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | INVALID_ID LP Args RP
    | INVALID_ID LP RP
    | ID LP Args error{ printf("missing closing parenthesis\n");} 
    | ID LP error { printf("missing closing parenthesis\n");}
    | INVALID_ID LP Args error{ printf("missing closing parenthesis\n");}
    | INVALID_ID LP error{ printf("missing closing parenthesis\n");}
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INVALID_ID
    | INT
    | FLOAT
    | CHAR
    | INVALID_TOKEN
    | Exp INVALID_TOKEN Exp 
    ;
Args: Exp COMMA Args
    | Exp
    ;
%%
int main(int argc, char**argv) {
    if (argc <= 1)
        return 1;
    FILE *f = fopen(argv[1], "r");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    yylineno = 0;
    // yydebug = 1;
    yyrestart(f);
    yyparse();
    return 0;
}
