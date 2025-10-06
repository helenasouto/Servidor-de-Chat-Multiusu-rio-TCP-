// src/ClientManager.cpp
#include "ClientManager.hpp"
#include <iostream>

void ClientManager::add_client(int socket_fd) {
    // Exclusão Mútua: Bloqueia o mutex para proteger a lista 
    const std::lock_guard<std::mutex> lock(mtx);
    
    active_clients.push_back(socket_fd);
    logger.log("Novo cliente adicionado. Total de clientes ativos: " + std::to_string(active_clients.size()));
}

void ClientManager::remove_client(int socket_fd) {
    // Exclusão Mútua: Bloqueia o mutex
    const std::lock_guard<std::mutex> lock(mtx);

    // Remove o socket da lista
    active_clients.erase(
        std::remove(active_clients.begin(), active_clients.end(), socket_fd),
        active_clients.end()
    );
    logger.log("Cliente removido. Total de clientes ativos: " + std::to_string(active_clients.size()));
}

void ClientManager::broadcast(int sender_fd, const std::string& message) {
    // Prepara a mensagem para broadcast (pode ser formatada, ex: [Cliente X]: mensagem)
    std::string full_message = "Cliente " + std::to_string(sender_fd) + ": " + message;
    
    // Exclusão Mútua: Acesso thread-safe à lista para iteração
    const std::lock_guard<std::mutex> lock(mtx);

    // Itera sobre todos os clientes conectados
    for (int client_socket : active_clients) {
        // Não envia a mensagem de volta para o remetente
        if (client_socket != sender_fd) {
            // Tenta enviar a mensagem
            if (send(client_socket, full_message.c_str(), full_message.length(), 0) < 0) {
                // Em caso de falha de envio (cliente inativo/desconectado)
                // A remoção será tratada pela thread do cliente quando ela tentar o próximo recv
                logger.log("ERRO ao enviar broadcast para o cliente: " + std::to_string(client_socket));
            }
        }
    }
}