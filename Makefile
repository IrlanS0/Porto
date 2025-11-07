# --- Configuração Portátil ---
CC = gcc
CFLAGS = -Wall -g -std=c99

# --- Detecção de Sistema (A Mágica) ---
EXECUTABLE = meu_programa
# Assume comandos Unix por padrão
RM = rm -f
RM_DIR = rm -rf
MKDIR_CMD = @mkdir -p $(OUTPUT_DIR)
EXEC_PREFIX = ./

# Se o 'make' detectar que está no Windows...
ifeq ($(OS),Windows_NT)
    EXECUTABLE := $(EXECUTABLE).exe
    RM = del /f /q
    RM_DIR = rmdir /s /q
    MKDIR_CMD = @mkdir $(OUTPUT_DIR) 2>NUL || exit 0
    EXEC_PREFIX = .\
else
	EXEC_PREFIX = ./
endif
# Fim do bloco ifeq (NÃO PODE ter espaço antes)

# --- Arquivos do Projeto ---
SRC_FILE = src/index.c
OUTPUT_DIR = saida_testes

# --- Descoberta Automática de Testes ---
INPUTS = $(wildcard testes/*.txt)
OUTPUTS = $(patsubst testes/%.txt, $(OUTPUT_DIR)/%.out, $(INPUTS))

# --- "Receitas" (Targets) ---
.PHONY: all test clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC_FILE)
	$(CC) $(CFLAGS) -o $@ $(SRC_FILE)

test: all $(OUTPUTS)
	@echo "--- Todos os testes foram gerados! ---"

$(OUTPUT_DIR)/%.out: testes/%.txt $(EXECUTABLE)
	$(MKDIR_CMD)
	@echo "Rodando teste: $< ..."
	$(EXEC_PREFIX)$(EXECUTABLE) $< $@

clean:
	@echo "Limpando..."
	@-$(RM) $(EXECUTABLE) 2>NUL
	@-$(RM_DIR) $(OUTPUT_DIR) 2>NUL
	@echo "Limpo!"