#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../lib/protocol.h"
#include "../lib/communication.h"


int main() {
    int sockfd = 0, rec, env, client, *length_array;
    int x[1];
    length_array = x;
    double array_answ[5];
    client = config_socket(local, remote, sockfd, PORT);
    struct request *request = (struct request*)malloc(sizeof(struct request));
    struct answer *answer = (struct answer*)malloc(sizeof(struct answer));

    while(TRUE){

        //reset_memory(request, answer);

        recv(client, length_array, sizeof(int), FLAGS);
        double *array = (double *)malloc(sizeof(double) * length_array[0]);

        rec = (int) recv(client, array, sizeof(double)*length_array[0], FLAGS);
        if(rec == -1){
            perror("\nErro no recebimento do pacote: ");
            return EXIT_FAILURE;
        }

        if (rec != FALSE){
            convert_array_struct(request, array);
            printf("Pacote recebido!\n\n");
            operate(request, answer);

            convert_struct_array(answer, array_answ, 5, TYPE_ANSWER);

            env = (int) send(client, array_answ, sizeof(double)*5, FLAGS);
            if(env == -1){
                perror("\nErro no envio do pacote: ");
                return EXIT_FAILURE;
            }
            show_data(request, answer);
            printf("Aguardando novo pacote...\n");
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