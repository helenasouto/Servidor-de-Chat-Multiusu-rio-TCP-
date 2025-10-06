// src/client.cpp - Versão multi-thread (CLI completo)
#include "tslog.hpp"
#include <iostream>
#include <string>
#include <thread>       // Para std::thread
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <atomic>       // Para controle de encerramento seguro

constexpr int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";
constexpr int BUFFER_SIZE = 1024;

// Variável atômica para sinalizar que o loop de recebimento deve parar.
// Usar std::atomic<bool> é essencial para sincronização simples entre threads (Sincronização / Exclusão Mútua).
std::atomic<bool> keep_running(true); 

// Função executada pela thread secundária: RECEBE MENSAGENS DO SERVIDOR
void receive_messages(int client_socket, ThreadSafeLogger& logger) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    // Loop que fica escutando o socket continuamente
    while (keep_running.load()) {
        // recv é uma chamada bloqueante, esperando a chegada de dados.
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            // Imprime a mensagem do servidor na tela
            // NOTA: É importante usar std::cout aqui para que a mensagem apareça em tempo real.
            std::cout << "\n[MENSAGEM] " << buffer << "\n> ";
            std::cout.flush(); // Garante que o prompt volte após a mensagem
            
        } else if (bytes_received == 0) {
            // Conexão encerrada pelo servidor
            logger.log("Conexão encerrada pelo servidor.");
            std::cerr << "Servidor fechou a conexão." << std::endl;
            keep_running.store(false);
            break;
            
        } else {
            // Erro de leitura, mas a thread principal pode já ter fechado o socket
            // (quebrando o loop com keep_running = false)
            if (keep_running.load()) {
                logger.log("ERRO: Falha ao receber dados do servidor. Desconectando.");
                std::cerr << "ERRO: Desconectado devido a uma falha de rede." << std::endl;
            }
            break; 
        }
        std::memset(buffer, 0, BUFFER_SIZE);
    }
    logger.log("Thread de recebimento finalizada.");
}
// src/client.cpp - CONTINUAÇÃO

// A partir daqui, cole a função main():

int main() {
    ThreadSafeLogger logger("chat_client.log");
    int client_socket = -1; // Inicializa com -1 para gerenciamento de erros (Gerenciamento de Recursos)

    // Tratamento de Erros: try/catch para a inicialização
    try {
        // 1. Criar e Conectar o socket
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) { 
            throw std::runtime_error("Falha ao criar o socket do cliente."); 
        }
        logger.log("Socket do cliente criado com sucesso.");

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

        if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) { 
            throw std::runtime_error("Falha ao conectar ao servidor."); 
        }

        logger.log("Conectado ao servidor com sucesso!");

        std::cout << "\n--- Conectado ao Servidor. Digite 'sair' para desconectar. ---\n" << std::endl; 

        // 2. Lançar a thread de Recebimento
        // Threads: Lança a thread secundária para receber mensagens de forma concorrente
        std::thread receiver_thread(receive_messages, client_socket, std::ref(logger));
        
        // 3. Loop principal para ENVIO de mensagens (Thread Principal)
        std::string input_line;
        while (keep_running.load()) {
            std::cout << "> ";
            // std::getline é uma chamada bloqueante, esperando a entrada do usuário
            if (!std::getline(std::cin, input_line)) {
                // Erro na leitura do console (ex: EOF), encerra
                keep_running.store(false);
                break;
            }

            if (input_line.empty()) continue;

            // Se o usuário digitar 'sair', encerra o cliente
            if (input_line == "sair") {
                keep_running.store(false);
                break;
            }

            // Enviar a mensagem
            if (send(client_socket, input_line.c_str(), input_line.length(), 0) < 0) {
                logger.log("ERRO: Falha ao enviar mensagem.");
                std::cerr << "Falha ao enviar mensagem. Desconectando." << std::endl;
                keep_running.store(false);
                break;
            }
        } 
        
        // 4. Encerramento (Gerenciamento de Recursos)
        
        // A thread principal fechou a conexão. 
        // Se a thread de recebimento ainda estiver bloqueada no recv(), isso a forçará a sair.
        shutdown(client_socket, SHUT_RDWR); 
        
        // Espera a thread de recebimento terminar (JOIN)
        if (receiver_thread.joinable()) {
            receiver_thread.join();
        }
        
    } catch (const std::exception& e) {
        // Tratamento de Erros: Exceções na inicialização ou durante a execução
        logger.log("ERRO FATAL DO CLIENTE: " + std::string(e.what()));
        std::cerr << "ERRO FATAL: " << e.what() << std::endl;
    }

    // Garante que o socket será fechado no final, mesmo após erros
    if (client_socket >= 0) {
        close(client_socket);
        logger.log("Conexão e socket fechados.");
    }

    return 0;
}