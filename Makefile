# Makefile para o projeto de Coletor de Lixo em C

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
EXEMPLOS_DIR = exemplos

# Arquivos fonte
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Arquivos de exemplo
EXEMPLO_SIMPLES = $(EXEMPLOS_DIR)/exemplo_simples.c
EXEMPLO_COMPLEXO = $(EXEMPLOS_DIR)/exemplo_complexo.c

# Cria diretórios necessários
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Regra padrão
all: lib exemplos

# Regra para criar a biblioteca
lib: $(OBJ_FILES)
	ar rcs $(BIN_DIR)/libgc.a $^

# Regra para compilar os objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar os exemplos
exemplos: $(BIN_DIR)/exemplo_simples $(BIN_DIR)/exemplo_complexo

$(BIN_DIR)/exemplo_simples: $(EXEMPLO_SIMPLES) lib
	$(CC) $(CFLAGS) $< -o $@ -L$(BIN_DIR) -lgc

$(BIN_DIR)/exemplo_complexo: $(EXEMPLO_COMPLEXO) lib
	$(CC) $(CFLAGS) $< -o $@ -L$(BIN_DIR) -lgc

# Regra para limpar o projeto
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Regra para executar os exemplos
run_simples: $(BIN_DIR)/exemplo_simples
	./$(BIN_DIR)/exemplo_simples

run_complexo: $(BIN_DIR)/exemplo_complexo
	./$(BIN_DIR)/exemplo_complexo

# Regra para executar todos os exemplos
run: run_simples run_complexo

.PHONY: all lib exemplos clean run run_simples run_complexo
