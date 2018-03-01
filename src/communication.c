#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../lib/communication.h"

int config_socket(struct sockaddr_in local, struct sockaddr_in remote, int sockfd, int port){
    socklen_t len = sizeof(remote);
    sockfd = socket(AF_INET, SOCK_STREAM, FLAGS);

    local.sin_family = AF_INET;
    local.sin_port = htons((uint16_t) port);
    memset(local.sin_zero, 0x0, SIZE_DOUBLE);

    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local))==-1){
        perror("bind ");
        exit(1);
    }

    if(listen(sockfd, 1)==-1){
        perror("listen ");
        exit(1);
    }

    printf("Aguardando requisição...\n");

    int client = accept(sockfd, (struct sockaddr *)&remote, &len);
    if(client==-1){
        perror("accept ");
        exit(1);
    }
    printf("Requisição conectado! Aguardando pacote...\n");
    return client;
}

void setup_socket(struct sockaddr_in *server, int port, char *ip){
    server->sin_family = AF_INET;
    server->sin_port = htons((uint16_t) port);
    inet_pton(AF_INET, ip, &remote.sin_addr);
    memset(server->sin_zero,0x0,SIZE_DOUBLE);
}