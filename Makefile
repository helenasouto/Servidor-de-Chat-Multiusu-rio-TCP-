# Makefile (versão multi-alvo)

# Compilador
CXX := g++

# Flags de compilação
CXXFLAGS := -std=c++17 -Wall -pthread -Iinclude

# Pastas
SRC_DIR := src
OBJ_DIR := build

# --- Definição dos nossos programas (alvos) ---

# Alvo 1: Servidor de Chat
SERVER_EXEC := chat_server
# *****************************************************************
# * MUDANÇA AQUI: Adiciona o arquivo de implementação do Monitor *
# *****************************************************************
SERVER_SRCS := $(SRC_DIR)/server.cpp $(SRC_DIR)/tslog.cpp $(SRC_DIR)/ClientManager.cpp
SERVER_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SERVER_SRCS))

# Alvo 2: Cliente do Chat (já preparado para quando criarmos o client.cpp)
CLIENT_EXEC := chat_client
CLIENT_SRCS := $(SRC_DIR)/client.cpp $(SRC_DIR)/tslog.cpp
CLIENT_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CLIENT_SRCS))

# Alvo 3: O teste original do logger
TEST_EXEC := test_logger
TEST_SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/tslog.cpp
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))


# --- Regras de Build ---

# Regra principal: 'make' ou 'make all' vai compilar o servidor e o cliente
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Regras de linkagem para cada executável
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_EXEC): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra genérica para compilar .cpp para .o
# Esta regra não muda, ela é inteligente e serve para todos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Comandos Utilitários ---

# Regra para compilar apenas o servidor
server: $(SERVER_EXEC)

# Regra para compilar apenas o cliente
client: $(CLIENT_EXEC)

# Regra para compilar apenas o teste de log
test_logger: $(TEST_EXEC)

# Regra de limpeza atualizada
clean:
	rm -rf $(OBJ_DIR) $(SERVER_EXEC) $(CLIENT_EXEC) $(TEST_EXEC) *.log

# Phony targets não são nomes de arquivos
.PHONY: all clean server client test_logger