#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.h"

int main()
{
    int sockfd, rec, env, client;
    char ip[10] = "127.0.0.1";

    /*Conexão com o socket requisição*/
   config_socket(local, remote, client, sockfd, PORT, ip);

    while(1){
        struct Request *request = (struct Request*)malloc(sizeof(struct Request));
        struct Answer *answer = (struct Answer*)malloc(sizeof(struct Answer));

		reset_memory(request, answer);

        rec = recv(client, request, sizeof(struct Request), 0);
        //("Tamanho RECV: %d Tamanho HEAD: %ld Tamanho Numeros: %ld\n\n", rec, sizeof(pac->head), sizeof(pac->numeros));
        if(rec > 0){
            printf("Pacote Recebido!\n\n");
            operation(request, answer);
            env = send(client, answer, sizeof(struct Answer), 0);
            show_data(request, answer);
            free(request);  request = NULL;
            free(answer); answer = NULL;
        }
		else{
            free(request); request = NULL;
            free(answer); answer = NULL;
            break;
    }
    printf("Encerrando protocolo!! \n");
    close(client);
    close(sockfd);
    return 0;
}
