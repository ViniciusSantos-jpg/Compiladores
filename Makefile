# Makefile para o Compilador C--

# Variáveis
CC = g++
CFLAGS = -Wall -Wextra
TARGET = cmm
SRC_DIR = Analise_Sintatica
TEST_DIR = testes

# Regra principal (o que roda ao digitar apenas 'make')
all: $(TARGET)

# Como compilar o executável (buscando os .cpp na pasta Analise_Sintatica)
$(TARGET):
	$(CC) $(CFLAGS) $(SRC_DIR)/*.cpp -o $(TARGET)
	@echo "Compilação concluída com sucesso! Executável '$(TARGET)' gerado na raiz."

# Regra para automatizar os testes (buscando na pasta testes)
test: all
	@echo "\n================================================="
	@echo " Iniciando bateria de testes..."
	@echo "================================================="
	@for file in $(TEST_DIR)/*.cmm; do \
		echo "\n---> Analisando: $$file"; \
		./$(TARGET) $$file; \
	done
	@echo "\n================================================="
	@echo " Testes finalizados."
	@echo "================================================="

# Regra para limpar os arquivos compilados
clean:
	rm -f $(TARGET)
	@echo "Executável removido."