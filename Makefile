# Makefile

# Compilador
CXX := g++

# Flags de compilação
# -std=c++17: Usa o padrão C++17
# -Wall: Mostra todos os warnings
# -pthread: Necessário para usar std::thread
# -Iinclude: Diz ao compilador para procurar headers na pasta 'include'
CXXFLAGS := -std=c++17 -Wall -pthread -Iinclude

# Pasta dos fontes e dos objetos
SRC_DIR := src
OBJ_DIR := build

# Arquivos fonte
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Arquivos objeto (compilados)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Nome do executável final
TARGET := test_logger

# Regra principal: o que fazer quando digitar 'make'
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar cada arquivo .cpp para um .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) # Cria a pasta 'build' se não existir
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(OBJ_DIR) $(TARGET) meu_log.txt

# Phony targets não são nomes de arquivos
.PHONY: all clean