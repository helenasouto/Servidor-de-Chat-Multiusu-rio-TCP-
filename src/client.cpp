// src/client.cpp - Versão que envia mensagem
#include "tslog.hpp"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

constexpr int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main() {
    ThreadSafeLogger logger("chat_client.log");

    // 1. Criar o socket do cliente
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        logger.log("ERRO: Falha ao criar o socket do cliente.");
        return 1;
    }
    logger.log("Socket do cliente criado com sucesso.");

    // 2. Preparar o endereço do servidor para conectar
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 3. Conectar ao servidor
    logger.log("Tentando conectar ao servidor em " + std::string(SERVER_IP) + ":" + std::to_string(PORT) + "...");
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        logger.log("ERRO: Falha ao conectar ao servidor.");
        close(client_socket);
        return 1;
    }

    logger.log("Conectado ao servidor com sucesso!");
    
    // ***** MUDANÇA AQUI *****
    // 4. Enviar uma mensagem para o servidor
    std::string message = "Ola, servidor! Sou o cliente.";
    logger.log("Enviando mensagem: " + message);
    
    if (send(client_socket, message.c_str(), message.length(), 0) < 0) {
        logger.log("ERRO: Falha ao enviar mensagem.");
    } else {
        std::cout << "Mensagem enviada com sucesso!" << std::endl;
    }
    // *************************

    // 5. Fechar a conexão
    close(client_socket);
    logger.log("Conexão fechada.");

    return 0;
}