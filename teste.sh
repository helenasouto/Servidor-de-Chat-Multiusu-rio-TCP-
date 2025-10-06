#!/bin/bash

# Função para limpar os processos em segundo plano ao sair
cleanup() {
    echo "Encerrando o servidor e os clientes..."
    # Encerra o processo do servidor
    kill $SERVER_PID
    # Garante que todos os processos de cliente sejam encerrados
    pkill chat_client
    echo "Limpeza concluída."
}

# Configura a função 'cleanup' para ser executada quando o script for encerrado (ex: com Ctrl+C)
trap cleanup EXIT

# 1. Compila o projeto (opcional, mas uma boa prática)
echo "Compilando o projeto..."
make

# 2. Inicia o servidor em segundo plano
echo "Iniciando o servidor..."
./chat_server &
SERVER_PID=$!

# Aguarda um instante para o servidor estar pronto
sleep 2

# 3. Simula múltiplos clientes
echo "Iniciando simulação com 3 clientes..."

# Cliente 1
(
    echo "Olá, sou a Helena!"
    sleep 3
    echo "O sistema de broadcast está funcionando?"
    sleep 2
    echo "sair"
) | ./chat_client &

# Aguarda um pouco antes de conectar o próximo cliente
sleep 2

# Cliente 2
(
    echo "Oi, Helena! Aqui é a Maria."
    sleep 4
    echo "Sim, está funcionando! Recebi sua mensagem."
    echo "sair"
) | ./chat_client &

sleep 1

# Cliente 3
(
    echo "Olá a todos! Sou o Cliente 3."
    echo "sair"
) | ./chat_client &


# 4. Mantém o script principal rodando por alguns segundos para a simulação acontecer
echo "Simulação em andamento por 10 segundos..."
sleep 10

# 5. A função de limpeza (cleanup) será chamada automaticamente ao final do script
echo "Fim da simulação."