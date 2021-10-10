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

int { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eTYPE;
    yylval.type_node->val.typeval = "int";
    return TYPE;
}

float {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eTYPE;
    yylval.type_node->val.typeval = "float";
    return TYPE;
}

char {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eTYPE;
    yylval.type_node->val.typeval = "char";
    return TYPE;
}

struct { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eSTRUCT;
    return STRUCT;
}

if { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eIF;
    return IF;
}
else {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eELSE;
    return ELSE;
}
while { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eWHILE;
    return WHILE;
}
return { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRETURN;
    return RETURN;
}
"." { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eDOT;
    return DOT;
}
";" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eSEMI;
    return SEMI;
}
"," {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eCOMMA;
    return COMMA;
}
"=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eASSIGN;
    return ASSIGN;
}
"<" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLT;
    return LT;
}
"<=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLE;
    return LE;
}
">" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eGT;
    return GT;
} 
">=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eGE;
    return GE;
}
"!=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eNE;
    return NE;
}
"==" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eEQ;
    return EQ;
}
"+" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = ePLUS;
    return PLUS;
}
"-" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eMINUS;
    return MINUS;
}
"*" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eMUL;
    return MUL;}
"/" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eDIV;
     return DIV;}
"&&" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eAND;
     return AND;}
"||" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eOR;
     return OR;}
"!" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eNOT;
     return NOT;}
"(" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLP;
    return LP;}
")" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRP;
     return RP;}
"[" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLB;
    return LB;}
"]" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRB;
     return RB;}
"{" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLC;
     return LC;}
"}" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRC;
    return RC;}


{int} {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eINT;
    yylval.type_node->val.intval = atoi(yytext);
     return INT; }

{int32} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eINT;
    yylval.type_node->val.intval = (int)strtol(yytext, NULL, 16); //TODO: possible bugs
    return INT;
}

{int_leading0} { 
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}

{int_invalid_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}

{int32_leading0} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}

{float} {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eFLOAT;
    yylval.type_node->val.floatval = atof(yytext); 
    return FLOAT; 
}

{char} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eCHAR;
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
    return INVALID_TOKEN;
}

{char_more_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}

{char_invalid_hex} {
    printf("Error type A at Line %d: unknown lexeme %s\n",
        yylineno, yytext);
    iserror = 1;
    return INVALID_TOKEN;
}

{id} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eID;
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
    return INVALID_ID;
}

"//" {
    char c = input();
    while (c != '\n') c = input();
}

"/*" {
    int c;
    for( ; ; ){
        while( (c = input()) != '*' && c != EOF)
            ;
        if ( c == '*' ){
            while ( (c = input()) == '*' )
                ;
            if ( c == '/')
                break;
        }
        if ( c == EOF ){
            perror("EOF in comment");
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