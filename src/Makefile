CC = gcc
CFLAGS = -Wall
LEX = flex

SCANNER_SRC = scanner.l
SCANNER_C = lex.yy.c
SCANNER_BIN = scanner_test

.PHONY: all scanner test clean

all: scanner

scanner: $(SCANNER_BIN)

$(SCANNER_BIN): $(SCANNER_C)
	$(CC) $(CFLAGS) -DSCANNER_TEST_MAIN $(SCANNER_C) -o $(SCANNER_BIN)

$(SCANNER_C): $(SCANNER_SRC)
	$(LEX) $(SCANNER_SRC)

test: scanner
	@for f in ../tests/validos/*.c; do \
		echo "== valido: $$f =="; \
		./$(SCANNER_BIN) $$f; \
		echo; \
	done
	@for f in ../tests/invalidos/*.c; do \
		echo "== invalido: $$f =="; \
		./$(SCANNER_BIN) $$f; \
		echo; \
	done

clean:
	rm -f $(SCANNER_C) $(SCANNER_BIN)
