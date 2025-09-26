// include/tslog.hpp
#ifndef TSLOG_HPP
#define TSLOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>

class ThreadSafeLogger {
public:
    // Construtor: abre o arquivo de log.
    ThreadSafeLogger(const std::string& filename);

    // Destrutor: fecha o arquivo.
    ~ThreadSafeLogger();

    // A função thread-safe para escrever no log.
    void log(const std::string& message);

    // Desabilitar cópia para evitar problemas com o mutex e o arquivo.
    ThreadSafeLogger(const ThreadSafeLogger&) = delete;
    ThreadSafeLogger& operator=(const ThreadSafeLogger&) = delete;

private:
    std::ofstream log_file;
    std::mutex mtx; // O mutex para proteger o acesso ao arquivo [cite: 43]
};

#endif // TSLOG_HPP