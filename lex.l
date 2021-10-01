%{
    #include"node.h"
    #include"syntax.tab.h"
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

int|float|char { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eTYPE;
    yylval.type_node->val.typeval = yytext;
    printf("TYPE %s\n", yytext); 
    return TYPE;
}
struct { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eSTRUCT;
    printf("STRUCT\n"); 
    return STRUCT;
}

if { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eIF;
    printf("IF\n");
    return IF;
}
else {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eELSE;
    printf("ELSE\n"); 
    return ELSE;
}
while { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eWHILE;
    printf("WHILE\n"); 
    return WHILE;
}
return { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRETURN;
    printf("RETURN\n"); 
    return RETURN;
}
"." { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eDOT;
    printf("DOT\n");
    return DOT;
}
";" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eSEMI;
    printf("SEMI\n");
    return SEMI;
}
"," {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eCOMMA;
    printf("COMMA\n"); 
    return COMMA;
}
"=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eASSIGN;
    printf("ASSIGN\n"); 
    return ASSIGN;
}
"<" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLT;
    printf("LT\n"); 
    return LT;
}
"<=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLE;
     printf("LE\n"); return LE;}
">" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eGT;
     printf("GT\n"); return GT;} 
">=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eGE;
     printf("GE\n"); return GE;}
"!=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eNE;
     printf("NE\n"); return NE;}
"==" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eEQ;
     printf("EQ\n"); return EQ;}
"+" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = ePLUS;
    printf("PLUS\n"); return PLUS;}
"-" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eMINUS;
    printf("MINUS\n"); return MINUS;}
"*" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eMUL;
    printf("MUL\n"); return MUL;}
"/" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eDIV;
     printf("DIV\n"); return DIV;}
"&&" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eAND;
     printf("AND\n"); return AND;}
"||" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eOR;
     printf("OR\n"); return OR;}
"!" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eNOT;
     printf("NOT\n"); return NOT;}
"(" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLP;
    printf("LP\n"); return LP;}
")" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRP;
     printf("RP\n"); return RP;}
"[" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLB;
    printf("LB\n"); return LB;}
"]" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRB;
     printf("RB\n"); return RB;}
"{" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eLC;
     printf("LC\n"); return LC;}
"}" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eRC;
    printf("RC\n"); return RC;}


{int} {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eINT;
    yylval.type_node->val.intval = atoi(yytext);
     printf("INT %s\n", yytext); return INT; }

{int32} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eINT;
    yylval.type_node->val.intval = atoi(yytext);
    printf("INT %s\n", yytext); return INT;}

{int_leading0} { 
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{int_invalid_hex} {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{int32_leading0} {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{float} {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eFLOAT;
    yylval.type_node->val.floatval = atof(yytext); 
     printf("FLOAT %s\n", yytext); return FLOAT; }

{char} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eCHAR;
    yylval.type_node->val.charval = *yytext;
    printf("CHAR %s\n", yytext); return CHAR; }

{char_double0s} {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{char_more_hex} {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{char_invalid_hex} {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}

{id} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->node_type = eID;
    yylval.type_node->val.idval = yytext;
    printf("ID %s\n", yytext); return ID; }

{wrong_id} {
    printf("Error type A at Line %d: Invalid Id: \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_ID;
}

\n {}
[\t\r ]+ {}
. {
    printf("Invalid_token, Error type A at Line %d: Mysterious characters \'%s\'\n",
        yylineno+1, yytext);
    return INVALID_TOKEN;
}
%%