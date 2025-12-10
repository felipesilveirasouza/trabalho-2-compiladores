SRCS = src/parser.y src/lexer.l src/tradutor_utils.c src/main.c
CC = gcc
CFLAGS = -Wall -g -std=gnu99 
INCLUDES = -Isrc -Itemp # CORREÇÃO: Adicionado -Itemp para encontrar parser.tab.h

# Nomes dos arquivos temporários e objetos
TEMP_DIR = temp
PARSER_C = $(TEMP_DIR)/parser.tab.c
PARSER_H = $(TEMP_DIR)/parser.tab.h
LEXER_C = $(TEMP_DIR)/lex.yy.c
UTILS_C = src/tradutor_utils.c
UTILS_H = src/tradutor_utils.h
MAIN_C = src/main.c

OBJS = $(TEMP_DIR)/parser.tab.o $(TEMP_DIR)/lex.yy.o $(TEMP_DIR)/tradutor_utils.o $(TEMP_DIR)/main.o
EXECUTABLE = $(TEMP_DIR)/json2maspy_translator

.PHONY: all setup clean

all: $(EXECUTABLE)
	@echo "--- Compilacao do tradutor concluida. ---"
	@echo "Execute o comando ./$(EXECUTABLE) tests/input/1.json"
	@echo "Depois execute python maspy_output.py"

# Regra principal para ligar o executável
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

# Regra para compilar o arquivo utilitário C em objeto
$(TEMP_DIR)/tradutor_utils.o: $(UTILS_C) $(UTILS_H)
	mkdir -p $(TEMP_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(UTILS_C) -o $(TEMP_DIR)/tradutor_utils.o

# Regra para compilar o main C em objeto
$(TEMP_DIR)/main.o: $(MAIN_C) $(UTILS_H)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(MAIN_C) -o $(TEMP_DIR)/main.o

# Regra para compilar o parser C em objeto
$(TEMP_DIR)/parser.tab.o: $(PARSER_C) $(PARSER_H) $(UTILS_H)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(PARSER_C) -o $(TEMP_DIR)/parser.tab.o

# Regra para compilar o lexer C em objeto
$(TEMP_DIR)/lex.yy.o: $(LEXER_C) $(UTILS_H)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(LEXER_C) -o $(TEMP_DIR)/lex.yy.o

# Regra para gerar os arquivos do Bison
$(PARSER_C) $(PARSER_H): src/parser.y
	mkdir -p $(TEMP_DIR)
	bison -d src/parser.y -o $(PARSER_C)

# Regra para gerar o arquivo do Flex
$(LEXER_C): src/lexer.l
	flex -o $(LEXER_C) src/lexer.l

setup:
	@echo "Configurando ambiente virtual Python..."
	test -d venv || python3 -m venv venv
	@echo "Ambiente virtual criado/existente em venv"
	@echo "AVISO IMPORTANTE: Para usar o ambiente virtual em seu terminal, voce DEVE ativa-lo manualmente:"
	@echo "  source venv/bin/activate"
	@echo "Certifique-se de instalar as dependencias Python (como 'maspy') dentro do ambiente virtual, se ainda nao o fez:"
	@echo "  venv/bin/pip install maspy"

clean:
	rm -rf $(TEMP_DIR)/* $(TEMP_DIR)
	rm -f tests/output/*.py
	rm -f src/parser.tab.c src/parser.tab.h src/lex.yy.c