// src/main.cpp
#include "tslog.hpp"
#include <thread> // Para usar std::thread [cite: 42]
#include <vector>
#include <sstream>

// Função que será executada por cada thread
void worker_task(ThreadSafeLogger& logger, int thread_id) {
    for (int i = 0; i < 5; ++i) {
        std::stringstream ss;
        ss << "Thread " << thread_id << " escrevendo a mensagem " << i;
        logger.log(ss.str());

        // Pequena pausa para simular algum trabalho e forçar a concorrência
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    try {
        // 1. Instancia o logger (que será compartilhado entre as threads)
        ThreadSafeLogger logger("meu_log.txt");
        logger.log("--- Programa de teste de log iniciado ---");

        // 2. Cria e armazena as threads
        const int NUM_THREADS = 10;
        std::vector<std::thread> threads;

        std::cout << "Iniciando " << NUM_THREADS << " threads para testar o logger..." << std::endl;

        for (int i = 0; i < NUM_THREADS; ++i) {
            // Lança uma nova thread executando a função worker_task
            threads.emplace_back(worker_task, std::ref(logger), i + 1);
        }

        // 3. Espera todas as threads terminarem (join)
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }

        logger.log("--- Teste de log finalizado ---");
        std::cout << "Teste finalizado. Verifique o arquivo 'meu_log.txt'." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl; // Tratamento de erros [cite: 47]
        return 1;
    }

    return 0;
}