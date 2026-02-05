#include <stdio.h>
#include <winsock2.h> // Padrao para sockets no windows

// Comando para o compilador do VS Code vincular a biblioteca fisica de rede
#pragma comment(lib, "ws2_32.lib") 

// Socket conecta IP, Porta e Protocolo(TCP)
// Para compilar gcc servidor.c -o servidor -lws2_32

int main(){
    // O código cria um servidor que escuta na porta 8080, aceita uma conexão, envia uma mensagem e fecha.
    // Inicialização do ambiente: exclusivo do windows
    WSADATA wsa; // Estrutura que guardará detalhes da implementação do Windows sockets
    // WSAStartup inicializa o uso da biblioteca. MAKEWORD(2,2) pede a versão 2.2
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Falha. Erro: %d", WSAGetLastError());
        return 1;
    }

    // socket(Familia de endereco, tipo, protocolo)
    // AF_INET: IPv4; 
    // SOCK_STREAM: TCP (conexao confiavel); 
    // 0: Escolha automatica do protocolo (geralmente IP)
    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s == INVALID_SOCKET) {
        printf("Nao foi possivel criar o socket");
    }

    struct sockaddr_in server; // Estrutura para configurar o IP e porta
    server.sin_family = AF_INET; // Define que usaremos IPv4
    server.sin_addr.s_addr = INADDR_ANY; // O servidor aceita conexoes de qualquer placa de rede do pc
    server.sin_port = htons(8080); // Porta 8080. htons converte o numero para "Network byte order", padroniza o formato

    // Ritual de conexao: Bind, Listen e Accept
    // Bind associa o socket 's' as configuracoes de ip/porta criadas
    bind(s, (struct sockaddr *)&server, sizeof(server)); // Casting do endereco de server

    // Listen coloca o socket em mode de espera. '3' eh o tamanho da fila de espera
    listen(s, 3);

    // Accept o programa "para" aqui e fica esperando alguem conectar
    // Quando alguem conecta, ele cria um NOVO socket (novo_socket) so para falar com esse cliente especifico
    int c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    SOCKET novo_socket = accept(s, (struct sockaddr *)&client, &c);

    // Agora a comunicacao
    char *msg = "Ola! voce se conectou.\n";

    // Send envia os bytes da string atraves do socket do cliente
    send(novo_socket, msg, strlen(msg), 0);

    // Limpeza obrigatoria para nao deixar lixo na memoria/rede
    closesocket(s); // Fecha o socket principal
    WSACleanup(); // Desliga a bib Winsock

    return 0;
}
