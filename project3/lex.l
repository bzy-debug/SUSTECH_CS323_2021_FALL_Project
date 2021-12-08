%{
    #include<string.h>
    #include"node.h"
    #include"syntax.tab.h"
    static int iserror = 0;

    int yycolno = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylineno; \
        yylloc.first_column = yycolno; \
        yylloc.last_line = yylineno; \
        yylloc.last_column = yycolno + yyleng - 1; \
        yycolno += yyleng;
%}

%option yylineno
d16 [0-9a-fA-F]
int 0|[1-9][0-9]*
int32 0[xX](0|[1-9a-fA-F]{d16}*)
int_leading0 0[0-9]+
int_invalid_hex 0[xX]{d16}*[g-zG-Z]+{d16}*
int32_leading0 0[xX](0{d16}+)
float [0-9]+\.[0-9]+
char '[^']'|'\\[xX](0|[1-9a-fA-F]{d16}{0,1})'
char_double0s '\\[xX]00'
char_more_hex '\\[xX]{d16}{3,}'
char_invalid_hex '\\[xX]{d16}*[^0-9a-fA-F]+{d16}*'
id [_a-zA-Z][_a-zA-Z0-9]*
wrong_id [0-9]+[_a-zA-Z]+[0-9]*
%%

read {
    yylval.type_node = create_grammar_node(eREAD, -1);
    return READ;
}

write {
    yylval.type_node = create_grammar_node(eWRITE, -1);
    return WRITE;
}

int { 
    yylval.type_node = create_grammar_node(eTYPE, -1);
    yylval.type_node->val.typeval = "int";
    return TYPE;
}

float {
    yylval.type_node = create_grammar_node(eTYPE, -1);
    yylval.type_node->val.typeval = "float";
    return TYPE;
}

char {
    yylval.type_node = create_grammar_node(eTYPE, -1);
    yylval.type_node->val.typeval = "char";
    return TYPE;
}

struct { 
    yylval.type_node = create_grammar_node(eSTRUCT, -1);
    return STRUCT;
}

if { 
    yylval.type_node = create_grammar_node(eIF, -1);
    return IF;
}
else {
    yylval.type_node = create_grammar_node(eELSE, -1);
    return ELSE;
}
while { 
    yylval.type_node = create_grammar_node(eWHILE, -1);
    return WHILE;
}
return { 
    yylval.type_node = create_grammar_node(eRETURN, -1);
    return RETURN;
}
"." { 
    yylval.type_node = create_grammar_node(eDOT, -1);
    return DOT;
}
";" { 
    yylval.type_node = create_grammar_node(eSEMI, -1);
    return SEMI;
}
"," {
    yylval.type_node = create_grammar_node(eCOMMA, -1);
    return COMMA;
}
"=" {
    yylval.type_node = create_grammar_node(eASSIGN, -1);
    return ASSIGN;
}
"<" {
    yylval.type_node = create_grammar_node(eLT, -1);
    return LT;
}
"<=" {
    yylval.type_node = create_grammar_node(eLE, -1);
    return LE;
}
">" {
    yylval.type_node = create_grammar_node(eGT, -1);
    return GT;
} 
">=" {
    yylval.type_node = create_grammar_node(eGE, -1);
    return GE;
}
"!=" {
    yylval.type_node = create_grammar_node(eNE, -1);
    return NE;
}
"==" {
    yylval.type_node = create_grammar_node(eEQ, -1);
    return EQ;
}
"+" {
    yylval.type_node = create_grammar_node(ePLUS, -1);
    return PLUS;
}
"-" {
    yylval.type_node = create_grammar_node(eMINUS, -1);
    return MINUS;
}
"*" {
    yylval.type_node = create_grammar_node(eMUL, -1);
    return MUL;
}
"/" {
    yylval.type_node = create_grammar_node(eDIV, -1);
    return DIV;
}
"&&" {
    yylval.type_node = create_grammar_node(eAND, -1);
    return AND;
}
"||" {
    yylval.type_node = create_grammar_node(eOR, -1);
    return OR;
}
"!" {
    yylval.type_node = create_grammar_node(eNOT, -1);
    return NOT;
}
"(" { 
    yylval.type_node = create_grammar_node(eLP, -1);
    return LP;
}
")" {
    yylval.type_node = create_grammar_node(eRP, -1);
    return RP;
}
"[" { 
    yylval.type_node = create_grammar_node(eLB, -1);
    return LB;
}
"]" {
    yylval.type_node = create_grammar_node(eRB, -1);
    return RB;
}
"{" {
    yylval.type_node = create_grammar_node(eLC, -1);
    return LC;
}
"}" { 
    yylval.type_node = create_grammar_node(eRC, -1);
    yylval.type_node->node_type = eRC;
    return RC;
}


{int} {
    yylval.type_node = create_grammar_node(eINT, -1);
    yylval.type_node->val.intval = atoi(yytext);
    return INT;
}

{int32} { 
    yylval.type_node = create_grammar_node(eINT, -1);
    yylval.type_node->val.intval = (int)strtol(yytext, NULL, 16); //TODO: possible bugs
    return INT;
}

{int_leading0} { 
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INT;
}

{int_invalid_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INT;
}

{int32_leading0} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INT;
}

{float} {
    yylval.type_node = create_grammar_node(eFLOAT, -1);
    yylval.type_node->val.floatval = atof(yytext); 
    return FLOAT; 
}

{char} { 
    yylval.type_node = create_grammar_node(eCHAR, -1);
    int l = strlen(yytext);
    yylval.type_node->val.charval = malloc(sizeof(char)*(l+1));
    for (int i=0; i<l; i++)
        yylval.type_node->val.charval[i] = yytext[i];
    yylval.type_node->val.charval[l] = '\0';
    return CHAR; 
}

{char_double0s} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return CHAR;
}

{char_more_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return CHAR;
}

{char_invalid_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return CHAR;
}

{id} { 
    yylval.type_node = create_grammar_node(eID, -1);
    int l = strlen(yytext);
    yylval.type_node->val.idval = malloc(sizeof(char)*(l+1));
    for(int i=0; i<l; i++)
        yylval.type_node->val.idval[i] = yytext[i];
    yylval.type_node->val.idval[l] = '\0';
    return ID; 
}

{wrong_id} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return ID;
}

"//" {
    char c = input();
    while (c != '\n') c = input();
}

"/*" {
    int c;
    for( ; ; ){
        c = input();
        while( c != '*' && c != 0)
            c = input();
        if ( c == '*' ){
            while ( (c = input()) == '*' )
                ;
            if ( c == '/')
                break;
        }
        if ( c == 0 ){
            printf("error: EOF in comment\n");
            iserror = 1;
            break;
        }
    }
}

\n { yycolno = 1; }
[\t\r ]+ {}
. {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}
%%