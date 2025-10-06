// include/ClientManager.hpp
#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <vector>
#include <mutex>
#include <algorithm>
#include <unistd.h> // Para a função send()
#include <sys/socket.h> 
#include "tslog.hpp" // Para logging

// Classe Monitor: Gerencia a lista de clientes de forma thread-safe
class ClientManager {
public:
    ClientManager(ThreadSafeLogger& logger) : logger(logger) {}
    
    // Adiciona um novo socket à lista
    void add_client(int socket_fd);

    // Remove um socket da lista (cliente desconectou)
    void remove_client(int socket_fd);

    // Envia uma mensagem para todos os clientes, exceto o remetente
    void broadcast(int sender_fd, const std::string& message);

    // Desabilitar cópia
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

private:
    std::vector<int> active_clients; // Estrutura compartilhada
    std::mutex mtx;                  // Mutex para exclusão mútua 
    ThreadSafeLogger& logger;        // Referência para o logger
};

#endif // CLIENT_MANAGER_HPP