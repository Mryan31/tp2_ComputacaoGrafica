# Compilador
CXX = g++

# Flags de compilacao
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Diretorios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos fonte e objeto
SRCS = \
OBJS = \

# Nome do executavel
TARGET = \/raytracer

# Regra padrao
all: \

# Linkagem
\: \
    \ \ -o \$@ \$pastas

# Compilacao dos objetos
\/%.o: \/%.cpp
    \ \ -c \$< -o \$@

# Limpeza
clean:
    rm -f \/*.o \
