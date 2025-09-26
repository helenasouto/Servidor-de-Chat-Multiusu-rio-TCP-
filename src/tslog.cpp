// src/tslog.cpp
#include "tslog.hpp"

ThreadSafeLogger::ThreadSafeLogger(const std::string& filename) {
    log_file.open(filename, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de log.");
    }
}

ThreadSafeLogger::~ThreadSafeLogger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void ThreadSafeLogger::log(const std::string& message) {
    // std::lock_guard é um mecanismo RAII que garante que o mutex será liberado
    // quando a função terminar, mesmo se ocorrer uma exceção. [cite: 84]
    const std::lock_guard<std::mutex> lock(mtx);

    // Pega o tempo atual para adicionar ao log
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    // Escreve a data, hora e a mensagem no arquivo
    log_file << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
             << " - " << message << std::endl;
}