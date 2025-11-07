# Define o compilador que vamos usar (gcc é o mais comum)
CC = gcc

# Flags de compilação (boas práticas: -Wall liga todos os warnings, -g inclui infos de debug)
CFLAGS = -Wall -g

# --- Nossos Arquivos ---

# Onde está o seu código fonte
SRC_FILE = src/index.c

# Onde está seu arquivo de entrada de testes
TEST_FILE = testes/testes.txt

# O nome que você quer dar ao seu executável (ele será criado na raiz)
EXECUTABLE = meu_programa

# Onde a saída deve ser salva
OUTPUT_DIR = saida_testes
OUTPUT_FILE = $(OUTPUT_DIR)/resultado.txt


# --- "Receitas" (Targets) ---

# 'all' é a receita padrão. Se você só digitar 'make', é ela que roda.
# Ela compila o fonte da 'src' e gera o executável na raiz.
all:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SRC_FILE)

# 'run' agora faz tudo: compila, cria a pasta de saída, e roda
# o programa redirecionando a entrada E a saída.
run: all
	@mkdir -p $(OUTPUT_DIR)
	./$(EXECUTABLE) < $(TEST_FILE) > $(OUTPUT_FILE)
	@echo "Feito! Resultado salvo em $(OUTPUT_FILE)"

# 'clean' agora também apaga a pasta de saídas.
clean:
	@echo "Limpando..."
	@rm -f $(EXECUTABLE)
	@rm -rf $(OUTPUT_DIR)
	@echo "Limpo!"