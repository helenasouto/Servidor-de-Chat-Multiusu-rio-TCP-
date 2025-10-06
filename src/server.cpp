// src/server.cpp - Versão multi-cliente (CONCORRENTE)
#include "tslog.hpp"
#include "ClientManager.hpp" // Novo include
#include <iostream>
#include <string>
#include <thread>           // Para std::thread
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>          // Para memset
#include <stdexcept>        // Para exceções

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

// Função executada por cada nova thread para atender um cliente
void handle_client(int client_socket, ClientManager& manager, ThreadSafeLogger& logger) {
    // Garante que o socket será fechado, mesmo em caso de erro (RAII simplificado)
    // Usar um wrapper de smart pointer seria ainda melhor para Gerenciamento de Recursos
    // Aqui, vamos apenas garantir o recurso principal:
    
    // 1. Adicionar o socket à lista do Monitor
    manager.add_client(client_socket);
    
    char buffer[BUFFER_SIZE];
    int bytes_received;

    try {
        // Loop de recebimento contínuo (o cliente fica conectado)
        while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';
            std::string raw_message(buffer);
            
            // Tratamento de mensagens vazias ou de controle (ex: 'sair')
            if (raw_message.empty()) continue;

            // 2. Broadcast da Mensagem (usando o Monitor)
            manager.broadcast(client_socket, raw_message);
            logger.log("Broadcast - Cliente [" + std::to_string(client_socket) + "]: " + raw_message);

            // Limpa o buffer para a próxima mensagem
            std::memset(buffer, 0, BUFFER_SIZE);

        }
        // Se recv retornar 0, o cliente fechou a conexão (desconexão normal)
        // Se retornar -1, ocorreu um erro
    } catch (const std::exception& e) {
        logger.log("Exceção na thread do cliente [" + std::to_string(client_socket) + "]: " + e.what());
    }

    // 3. Remover o cliente da lista e fechar o socket
    manager.remove_client(client_socket);
    close(client_socket);
    logger.log("Conexão com o cliente [" + std::to_string(client_socket) + "] fechada. Thread finalizada.");
}

int main() {
    // Tratamento de Erros: Usando try/catch para inicialização crítica
    try {
        ThreadSafeLogger logger("chat_server.log");
        
        // 1. Instancia o Monitor (compartilhado por todas as threads)
        ClientManager client_manager(logger); 

        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) { 
            throw std::runtime_error("Falha ao criar o socket do servidor."); 
        }
        logger.log("Socket do servidor criado com sucesso.");

        int opt = 1;
        // Gerenciamento de Recursos: Permite reutilizar o endereço imediatamente
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { 
            throw std::runtime_error("Falha ao configurar SO_REUSEADDR."); 
        }

        // ... (configuração do endereço e bind)
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
            throw std::runtime_error("Falha ao vincular o socket à porta."); 
        }
        logger.log("Socket vinculado a porta " + std::to_string(PORT));

        if (listen(server_fd, 5) < 0) { 
            throw std::runtime_error("Falha ao escutar na porta."); 
        }
        logger.log("Servidor escutando na porta " + std::to_string(PORT) + "...");

        // Loop principal para aceitar conexões
        while (true) {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

            if (client_socket < 0) {
                logger.log("ERRO: Falha ao aceitar conexão do cliente. Continuando...");
                continue; 
            }
            logger.log("Nova conexão aceita: socket " + std::to_string(client_socket));

            // ***** Concorrência: Lança uma nova thread para atender o cliente *****
            // Threads: Uso de std::thread para concorrer clientes
            std::thread client_thread(handle_client, client_socket, std::ref(client_manager), std::ref(logger));
            
            // Permite que a thread rode de forma independente
            client_thread.detach(); 
        } 

    } catch (const std::exception& e) {
        // Tratamento de Erros: Captura exceções na inicialização
        std::cerr << "ERRO FATAL DO SERVIDOR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}