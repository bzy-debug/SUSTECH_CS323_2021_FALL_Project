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
    yylval.type_node->info = yytext;
    printf("TYPE %s\n", yytext); 
    return TYPE;
}
struct { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("STRUCT\n"); 
    return STRUCT;
}

if { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("IF\n");
    return IF;
}
else {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("ELSE\n"); 
    return ELSE;
}
while { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("WHILE\n"); 
    return WHILE;
}
return { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("RETURN\n"); 
    return RETURN;
}
"." { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("DOT\n"); return DOT;}
";" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("SEMI\n"); return SEMI;}
"," {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("COMMA\n"); return COMMA;}
"=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("ASSIGN\n"); return ASSIGN;}
"<" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("LT\n"); return LT;}
"<=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("LE\n"); return LE;}
">" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("GT\n"); return GT;} 
">=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("GE\n"); return GE;}
"!=" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("NE\n"); return NE;}
"==" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("EQ\n"); return EQ;}
"+" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("PLUS\n"); return PLUS;}
"-" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("MINUS\n"); return MINUS;}
"*" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("MUL\n"); return MUL;}
"/" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("DIV\n"); return DIV;}
"&&" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("AND\n"); return AND;}
"||" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("OR\n"); return OR;}
"!" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("NOT\n"); return NOT;}
"(" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("LP\n"); return LP;}
")" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("RP\n"); return RP;}
"[" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("LB\n"); return LB;}
"]" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("RB\n"); return RB;}
"{" {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("LC\n"); return LC;}
"}" { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
    printf("RC\n"); return RC;}


{int} {
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
     printf("INT %s\n", yytext); return INT; }

{int32} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
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
    yylval.type_node->info = yytext;
     printf("FLOAT %s\n", yytext); return FLOAT; }

{char} { 
    yylval.type_node = malloc(sizeof(node));
    yylval.type_node->info = yytext;
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
    yylval.type_node->info = yytext;
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