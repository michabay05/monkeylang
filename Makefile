COMP_FLAG = -Wall -Wextra -pedantic

lexer_test: src/test/lexer_test.c src/token.c src/lexer.c
	gcc $(COMP_FLAG) -ggdb -o bin/lexer_test src/test/lexer_test.c src/token.c src/lexer.c
