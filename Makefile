# --- Configurações do Compilador ---
CXX      := g++
# -std=c++17: Padrão moderno (funciona com c++11 também)
# -O3: Otimização máxima (essencial para Ray Tracing ser rápido)
# -Wall: Avisos de boas práticas
# -Iinclude: Permite usar #include <arquivo.hpp> se quiser, mas funciona com aspas também
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -Iinclude

# --- Diretórios ---
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# --- Arquivos ---
# Encontra todos os arquivos .cpp dentro de src/
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Define o nome dos objetos baseados nos nomes dos fontes (src/xxx.cpp -> obj/xxx.o)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# --- Nome do Executável Final ---
TARGET := $(BIN_DIR)/raytracer.exe

# --- Regras (Targets) ---

# Regra padrão (o que roda quando você digita apenas 'make')
all: directories $(TARGET)

# Cria as pastas de saída se não existirem (Sintaxe Windows CMD)
directories:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@if not exist "outputs" mkdir "outputs"

# Linkagem: Junta todos os .o para criar o .exe
$(TARGET): $(OBJS)
	@echo [LINK] Gerando executavel: $@
	@$(CXX) $(CXXFLAGS) $^ -o $@

# Compilação: Transforma cada .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CXX] Compilando: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza: Apaga arquivos gerados (Sintaxe Windows)
clean:
	@echo Limpando arquivos temporarios...
	@if exist "$(OBJ_DIR)\*.o" del /Q "$(OBJ_DIR)\*.o"
	@if exist "$(TARGET)" del /Q "$(TARGET)"
	@echo Limpeza concluida.

# Atalho para rodar (Exemplo)
# Uso: make run
run: all
	@echo Rodando Ray Tracer...
	@$(TARGET) inputs/exemplo.txt outputs/teste.ppm 800 600