#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.c"

int main()
{
    int sockfd, rec, env, client;
    char ip[10] = "127.0.0.1";
    socklen_t len = sizeof(remote);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    memset(local.sin_zero, 0x0, 8);

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }

    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local))==-1){
        perror("bind ");
        exit(1);
    }

     if(listen(sockfd, 1)==-1){
		perror("listen ");
		exit(1);
	}

	printf("Aguardando requisição...\n");

    client = accept(sockfd, (struct sockaddr *)&remote, &len);
    if(client==-1){
        perror("accept ");
        exit(1);
    }
    printf("Requisição conectado! Aguardando pacote...\n");

    while(1){
        struct Request *request = (struct Request*)malloc(sizeof(struct Request));
        struct Answer *answer = (struct Answer*)malloc(sizeof(struct Answer));

		reset_memory(request, answer);

        rec = recv(client, request, sizeof(struct Request), 0);
        /*("Tamanho RECV: %d Tamanho HEAD: %ld Tamanho Numeros: %ld\n\n", rec, sizeof(pac->head), sizeof(pac->numeros));*/
        if(rec > 0){
            printf("Pacote Recebido!\n\n");
            operation(request, answer);
            env = (int) send(client, answer, sizeof(struct Answer), 0);
            show_data(request, answer);
            free(request);  request = NULL;
            free(answer); answer = NULL;
        }
		else {
            free(request);
            request = NULL;
            free(answer);
            answer = NULL;
            break;
        }
    }
    printf("Encerrando protocolo!! \n");
    close(client);
    close(sockfd);
    return 0;
}
