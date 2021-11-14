CC = gcc
FLEX = flex
BISON = bison

.lex: lex.l
	$(FLEX) lex.l
.syntax: syntax.y
	$(BISON) -d -t -v syntax.y
splc: .lex .syntax
	mkdir bin
	$(CC) -g main.c node.c type.c llist.c llist_node.c symbol_table.c -lfl -ly -o bin/splc
assert: test.c
	$(CC) -g test.c type.c -o bin/test
	./bin/test
clean:
	@rm -f syntax.tab.h lex.yy.c syntax.tab.c *.out syntax.output
	@rm -rf ./bin