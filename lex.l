%{
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

int|float|char { printf("TYPE %s\n", yytext); return TYPE;}
struct { printf("STRUCT\n"); return STRUCT;}
if { printf("IF\n"); return IF;}
else { printf("ELSE\n"); return ELSE;}
while { printf("WHILE\n"); return WHILE;}
return { printf("RETURN\n"); return RETURN;}
"." { printf("DOT\n"); return DOT;}
";" { printf("SEMI\n"); return SEMI;}
"," { printf("COMMA\n"); return COMMA;}
"=" { printf("ASSIGN\n"); return ASSIGN;}
"<" { printf("LT\n"); return LT;}
"<=" { printf("LE\n"); return LE;}
">" { printf("GT\n"); return GT;} 
">=" { printf("GE\n"); return GE;}
"!=" { printf("NE\n"); return NE;}
"==" { printf("EQ\n"); return EQ;}
"+" { printf("PLUS\n"); return PLUS;}
"-" { printf("MINUS\n"); return MINUS;}
"*" { printf("MUL\n"); return MUL;}
"/" { printf("DIV\n"); return DIV;}
"&&" { printf("AND\n"); return AND;}
"||" { printf("OR\n"); return OR;}
"!" { printf("NOT\n"); return NOT;}
"(" { printf("LP\n"); return LP;}
")" { printf("RP\n"); return RP;}
"[" { printf("LB\n"); return LB;}
"]" { printf("RB\n"); return RB;}
"{" { printf("LC\n"); return LC;}
"}" { printf("RC\n"); return RC;}


{int} { printf("INT %s\n", yytext); return INT; }

{int32} { printf("INT %s\n", yytext); return INT;}

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

{float} { printf("FLOAT %s\n", yytext); return FLOAT; }

{char} { printf("CHAR %s\n", yytext); return CHAR; }

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

{id} { printf("ID %s\n", yytext); return ID; }

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