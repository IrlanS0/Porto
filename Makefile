# --- Configuração (Somente Windows) ---
CC = gcc
CFLAGS = -Wall -g -std=c99

# --- Arquivos do Projeto ---
EXECUTABLE = meu_programa.exe
SRC_FILE = src/index.c
OUTPUT_DIR = saida_testes

# --- Descoberta Automática de Testes ---
INPUTS = $(wildcard testes/*.txt)
OUTPUTS = $(patsubst testes/%.txt, $(OUTPUT_DIR)/%.out, $(INPUTS))

# --- "Receitas" (Targets) ---
.PHONY: all test clean

# 'all' (padrão)
all: $(EXECUTABLE)

# Compila o executável
$(EXECUTABLE): $(SRC_FILE)
	$(CC) $(CFLAGS) -o $@ $(SRC_FILE)

# 'test' depende de 'all' e de todas as saídas
test: all $(OUTPUTS)
	@echo "--- Todos os testes foram gerados! ---"

# --- A REGRA MÁGICA ---
$(OUTPUT_DIR)/%.out: testes/%.txt $(EXECUTABLE)
	@mkdir $(OUTPUT_DIR) 2>NUL || exit 0
	@echo "Rodando teste: $< ..."
	.\$(EXECUTABLE) $< $@

# 'clean': Limpa tudo
clean:
	@echo "Limpando..."
	@-del /f /q $(EXECUTABLE) 2>NUL
	@-rmdir /s /q $(OUTPUT_DIR) 2>NUL
	@echo "Limpo!"