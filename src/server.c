#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "protocol.h"
#include "communication.h"


int main()
{
    int sockfd, rec, env, client;
    client = config_socket(local, remote, sockfd, PORT);
    //struct request *request = (struct request*)malloc(sizeof(struct request));
    //struct answer *answer = (struct answer*)malloc(sizeof(struct answer));

	while(TRUE){

        struct request *request = (struct request*)malloc(sizeof(struct request));
        struct answer *answer = (struct answer*)malloc(sizeof(struct answer));
    	

    	//reset_memory(request, answer);
        rec = recv(client, request, sizeof(struct request), FLAGS);
        if (rec != FALSE){
            printf("Pacote Recebido!\n\n");
            operate(request, answer);
            env = (int) send(client, answer, sizeof(struct answer), FLAGS);
            show_data(request, answer);
            printf("Aguardando novo pacote...\n");
            free(request);
            request = NULL;
            free(answer);
            answer = NULL;
        }
        
        else{
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
