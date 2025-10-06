#!/bin/bash

# Verifica se os argumentos necessários foram fornecidos
if [ "$#" -ne 3 ]; then
    echo "Uso: $0 <host> <porta> <numero_de_clientes>"
    exit 1
fi

# Parâmetros
HOST=$1
PORT=$2
NUM_CLIENTS=$3
CLIENT_EXECUTABLE="./chat_client"

# Verifica se o executável do cliente existe
if [ ! -f "$CLIENT_EXECUTABLE" ]; then
    echo "Erro: Executável do cliente ($CLIENT_EXECUTABLE) não encontrado."
    echo "Por favor, compile o projeto primeiro com 'make'."
    exit 1
fi

echo "Iniciando teste de estresse com ${NUM_CLIENTS} clientes simultâneos..."
echo "Host: ${HOST}, Porta: ${PORT}"
echo "-----------------------------------------------------"

# Loop para iniciar todos os clientes em segundo plano
for i in $(seq 1 $NUM_CLIENTS)
do
    # Cada cliente envia uma mensagem de identificação, espera um pouco e sai.
    # O '&' no final executa o processo em segundo plano.
    (
        # Pequeno atraso aleatório para não conectar todos exatamente no mesmo instante
        sleep "0.$((RANDOM % 5))" 
        echo "Olá, servidor! Eu sou o Cliente ${i} de ${NUM_CLIENTS}."
        sleep 1
        echo "sair"
    ) | $CLIENT_EXECUTABLE $HOST $PORT > /dev/null 2>&1 &
    # A saída é redirecionada para /dev/null para não poluir o terminal
done

# Aguarda todos os processos em segundo plano (clientes) terminarem
wait

echo "-----------------------------------------------------"
echo "Teste de estresse concluído. Todos os ${NUM_CLIENTS} clientes finalizaram."