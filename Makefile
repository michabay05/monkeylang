COMP_FLAG = -Wall -Wextra -pedantic -std=c11 -ggdb

lexer_test: src/test/lexer_test.c src/token.c src/lexer.c src/misc.c
	gcc $(COMP_FLAG) -o bin/lexer_test src/test/lexer_test.c src/token.c src/lexer.c src/misc.c

repl: src/repl.c src/token.c src/lexer.c src/misc.c
	gcc $(COMP_FLAG) -o bin/repl src/repl.c src/token.c src/lexer.c src/misc.c

parser_test: src/test/parser_test.c src/token.c src/lexer.c src/misc.c src/ast.c src/parser.c
	gcc $(COMP_FLAG) -o bin/parser_test src/test/parser_test.c src/token.c src/lexer.c src/misc.c src/ast.c src/parser.c

ast_test: src/test/ast_test.c src/token.c src/lexer.c src/misc.c src/ast.c src/parser.c
	gcc $(COMP_FLAG) -o bin/ast_test src/test/ast_test.c src/token.c src/lexer.c src/misc.c src/ast.c src/parser.c
