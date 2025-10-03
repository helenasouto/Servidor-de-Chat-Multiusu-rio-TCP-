// src/server.cpp - Versão multi-cliente (sequencial)
#include "tslog.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
    ThreadSafeLogger logger("chat_server.log");

    // Passos 1, 1.5, 2 e 3 continuam os mesmos...
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) { /* ... erro ... */ return 1; }
    logger.log("Socket do servidor criado com sucesso.");

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) { /* ... erro ... */ return 1; }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { /* ... erro ... */ return 1; }
    logger.log("Socket vinculado a porta " + std::to_string(PORT));

    if (listen(server_fd, 5) < 0) { /* ... erro ... */ return 1; }
    logger.log("Servidor escutando na porta " + std::to_string(PORT) + "...");

    // ***** MUDANÇA AQUI: Início do loop infinito *****
    while (true) {
        // 4. Aceitar uma conexão
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            logger.log("ERRO: Falha ao aceitar conexão do cliente. Continuando...");
            continue; // Se falhar, apenas espera o próximo
        }
        logger.log("Cliente conectado!");

        // 5. Receber uma mensagem do cliente
        char buffer[BUFFER_SIZE] = {0};
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received > 0) {
            logger.log("Mensagem recebida do cliente: " + std::string(buffer));
        } else {
            logger.log("Cliente desconectou ou ocorreu um erro.");
        }
        
        // 6. Fechar a conexão DESTE cliente
        close(client_socket);
        logger.log("Conexão com o cliente fechada. Aguardando próximo...");
    } // ***** Fim do loop, volta para o accept() *****

    // O código abaixo nunca será alcançado neste modelo simples,
    // o servidor é parado pelo script de teste.
    close(server_fd);
    logger.log("Servidor finalizado.");

    return 0;
}