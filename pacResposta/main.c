#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.c"

int main()
{
    int sockfd, rec, env, client;
    client = config_socket(local, remote, sockfd, PORT);

    while(1){
        struct Request *request = (struct Request*)malloc(sizeof(struct Request));
        struct Answer *answer = (struct Answer*)malloc(sizeof(struct Answer));

		reset_memory(request, answer);

        rec = recv(client, request, sizeof(struct Request), FLAGS);
        /*("Tamanho RECV: %d Tamanho HEAD: %ld Tamanho Numeros: %ld\n\n", rec, sizeof(pac->head), sizeof(pac->numeros));*/
        if (rec > 0) {
            printf("Pacote Recebido!\n\n");
            operation(request, answer);
            env = (int) send(client, answer, sizeof(struct Answer), FLAGS);
            show_data(request, answer);
            free(request);
            request = NULL;
            free(answer);
            answer = NULL;
        } else {
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
