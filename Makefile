CC = gcc
FLEX = flex
BISON = bison

.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -d -t -v syntax.y
splc: .lex .syntax
	$(CC) -g main.c node.c -lfl -ly -o bin/splc
clean:
	@rm -f syntax.tab.h lex.yy.c syntax.tab.c *.out syntax.output