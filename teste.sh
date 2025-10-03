#!/bin/bash

# Script de teste para o protótipo do chat

echo "--- Iniciando teste de comunicação ---"

# Limpa os logs antigos
rm -f chat_server.log chat_client.log

echo "[TESTE] Compilando o projeto..."
make clean
make

echo "[TESTE] Iniciando o servidor em background..."
./chat_server &
SERVER_PID=$! # Salva o ID do processo do servidor

# Pequena pausa para garantir que o servidor está pronto
sleep 1

echo "[TESTE] Iniciando 3 clientes em sequência..."
./chat_client
./chat_client
./chat_client

echo "[TESTE] Teste dos clientes finalizado."

# Para o servidor que estava em background
kill $SERVER_PID
echo "[TESTE] Servidor parado."

echo "--- Teste finalizado. Verifique os logs. ---"