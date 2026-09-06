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
PARSER_BIN = parser_test

.PHONY: all parser test clean

all: parser

parser: $(PARSER_BIN)

$(PARSER_C): $(PARSER_SRC)
	$(YACC) -d $(PARSER_SRC)

$(PARSER_H): $(PARSER_C)
	@true

$(SCANNER_C): $(SCANNER_SRC) $(PARSER_H)
	$(LEX) $(SCANNER_SRC)

$(PARSER_BIN): $(PARSER_C) $(SCANNER_C) $(AST_SRC)
	$(CC) $(CFLAGS) -DPARSER_TEST_MAIN $(PARSER_C) $(SCANNER_C) $(AST_SRC) -o $(PARSER_BIN)

test: parser
	@for f in ../tests/validos/*.c; do \
		echo "== valido: $$f =="; \
		./$(PARSER_BIN) $$f; \
		echo; \
	done
	@for f in ../tests/invalidos/*.c; do \
		echo "== invalido: $$f =="; \
		./$(PARSER_BIN) $$f; \
		echo; \
	done

clean:
	rm -f $(SCANNER_C) $(PARSER_C) $(PARSER_H) parser.output $(PARSER_BIN)
