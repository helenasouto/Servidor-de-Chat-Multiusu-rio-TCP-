# Projeto Final - Servidor de Chat Concorrente

**Disciplina:** Programação Concorrente (C/C++)
**Aluno(a):** Helena Souto
**Professor:** Bidu

## 📄 Descrição do Projeto

Este projeto consiste na implementação de um sistema de chat multiusuário (Tema A), desenvolvido como avaliação final da disciplina. O sistema é composto por um servidor TCP concorrente e um cliente de linha de comando (CLI), ambos escritos em C++17.

O foco do projeto foi a aplicação prática de conceitos de programação concorrente, como threads, exclusão mútua (mutexes), o padrão Monitor para gerenciamento de estado compartilhado, e comunicação em rede via Sockets.

## ✨ Funcionalidades

- **Servidor TCP Concorrente:** Capaz de gerenciar múltiplas conexões de clientes simultaneamente.
- **Modelo Thread-por-Cliente:** Cada cliente conectado é gerenciado por uma `std::thread` dedicada no servidor.
- **Broadcast de Mensagens:** Mensagens enviadas por um cliente são retransmitidas para todos os outros clientes conectados em tempo real.
- **Cliente CLI Não-Bloqueante:** O cliente utiliza threads separadas para envio (entrada do usuário) e recebimento de mensagens, proporcionando uma experiência de uso fluida.
- **Logging Thread-Safe:** Todos os eventos importantes do servidor e do cliente são registrados em arquivos de log de forma segura, utilizando uma biblioteca de logging (`libtslog`) baseada no padrão Monitor.
- **Gerenciamento Seguro de Estado:** A lista de clientes conectados no servidor é protegida contra condições de corrida usando um `std::mutex` encapsulado na classe `ClientManager` (Padrão Monitor).

## 🗂️ Estrutura do Repositório
.
├── build/              # Contém os arquivos de objeto (.o) gerados durante a compilação.
├── doc/                # Contém diagramas e artefatos das etapas iniciais do projeto.
├── include/            # Contém os arquivos de cabeçalho (.hpp) do projeto.
├── src/                # Contém os arquivos de implementação (.cpp) do projeto.
├── relatorio_final/    # Contém a documentação final da Etapa 3 (relatórios, diagrama final).
├── Makefile            # Script de compilação que automatiza o build do projeto.
├── teste.sh            # Script para teste de simulação simples.
├── stress.sh           # Script para teste de estresse com múltiplos clientes.
├── chat_server         # Executável do servidor (gerado após o 'make').
├── chat_client         # Executável do cliente (gerado após o 'make').
├── chat_server.log     # Arquivo de log gerado pelo servidor.
├── chat_client.log     # Arquivo de log gerado pelo cliente.
└── README.md           # Este arquivo de documentação.


## 🚀 Como Compilar e Executar

O projeto utiliza um `Makefile` para automatizar a compilação.

**1. Compilar o projeto:**

```bash
# Opcional: limpa os arquivos de compilação anteriores
make clean

# Compila o servidor e o cliente
make

2. Executar o Servidor:

Abra um terminal e execute:
Bash

./chat_server

O servidor iniciará e aguardará por conexões na porta 8080.

3. Executar o Cliente:

Abra um novo terminal para cada cliente que desejar conectar. Execute:
Bash

./chat_client

🧪 Como Executar os Testes

O projeto inclui scripts para automatizar os testes de simulação e estresse.

1. Teste de Simulação Simples:

Este script inicia o servidor e simula 3 clientes trocando mensagens.
Bash

# Dê permissão de execução (apenas na primeira vez)
chmod +x teste.sh

# Execute o script
./teste.sh

2. Teste de Estresse:

Este script simula a conexão simultânea de múltiplos clientes. O servidor precisa estar rodando em outro terminal.
Bash

# Dê permissão de execução (apenas na primeira vez)
chmod +x stress.sh

# Execute o teste com 50 clientes
./stress.sh 127.0.0.1 8080 50