CC = gcc
CFLAGS = -Wall
LEX = flex
YACC = bison

PARSER_SRC = parser.y
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
SCANNER_SRC = scanner.l
INTERPRETER_SRC = interpreter.c
SCANNER_C = lex.yy.c
AST_SRC = ast.c
TABELA_SRC = tabela.c
SEMANTIC_SRC = semantic.c
MAIN_SRC = main.c
BIN = interpretador

.PHONY: all test clean

all: $(BIN)

$(PARSER_C): $(PARSER_SRC)
	$(YACC) -d $(PARSER_SRC)

$(PARSER_H): $(PARSER_C)
	@true

$(SCANNER_C): $(SCANNER_SRC) $(PARSER_H)
	$(LEX) $(SCANNER_SRC)

$(BIN): $(MAIN_SRC) $(PARSER_C) $(SCANNER_C) $(AST_SRC) $(TABELA_SRC) $(SEMANTIC_SRC) $(INTERPRETER_SRC)
	$(CC) $(CFLAGS) $(MAIN_SRC) $(PARSER_C) $(SCANNER_C) $(AST_SRC) $(TABELA_SRC) $(SEMANTIC_SRC) $(INTERPRETER_SRC) -o $(BIN)

test: $(BIN)
	@bash run_tests.sh

clean:
	rm -f $(SCANNER_C) $(PARSER_C) $(PARSER_H) parser.output $(BIN)

