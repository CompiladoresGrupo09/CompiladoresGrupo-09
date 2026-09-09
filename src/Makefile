CC = gcc
CFLAGS = -Wall
LEX = flex
YACC = bison

PARSER_SRC = parser.y
PARSER_C = parser.tab.c
PARSER_H = parser.tab.h
SCANNER_SRC = scanner.l
SCANNER_C = lex.yy.c
AST_SRC = ast.c
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

$(BIN): $(MAIN_SRC) $(PARSER_C) $(SCANNER_C) $(AST_SRC)
	$(CC) $(CFLAGS) $(MAIN_SRC) $(PARSER_C) $(SCANNER_C) $(AST_SRC) -o $(BIN)

test: $(BIN)
	@for f in ../tests/validos/*.c; do \
		echo "== valido: $$f =="; \
		if ! ./$(BIN) $$f; then \
			echo "ERRO: esperado sucesso (sem erro) para $$f"; exit 1; \
		fi; \
		echo; \
	done
	@for f in ../tests/invalidos/*.c; do \
		echo "== invalido: $$f =="; \
		if ./$(BIN) $$f; then \
			echo "ERRO: esperado falha (erro lexico ou sintatico) para $$f"; exit 1; \
		fi; \
		echo; \
	done

clean:
	rm -f $(SCANNER_C) $(PARSER_C) $(PARSER_H) parser.output $(BIN)

