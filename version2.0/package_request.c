#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <endian.h>
#include "protocol.c"


#define PORTA 3000


struct sockaddr_in remote;

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec;
    int env;
    int id = 0;
    int op;
    int num;
    int checker = TRUE;
    double valor;
    char ip[32];
    struct Cell *pont_ini = NULL, *pont_fim = NULL;
    struct Request *request;
    struct Answer *answer;
    socklen_t len = sizeof(remote);

    printf("Digite o Ip desejado para conexao: ");
    fgets(ip,14,stdin);// 192.168.1.120-> lucas

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("\nSocket criado!\n");
    }

    setup_socket(&remote,PORTA,ip);

    if(connect(sockfd, (struct sockaddr *)&remote, len) < 0){
        perror("connect ");
        exit(1);
    }

    printf("Conectado!\n\n");

    while(checker){
        //printf("\e[H\e[2J");
        //printf("\33[H\33[2J");  prints que limpam o console
        op = menu();
        switch(op){
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE:
                request = (struct Request *)malloc(sizeof(struct Request));
                answer = (struct Answer *)malloc(sizeof(struct Answer));
                if(pont_ini == NULL){
                    pont_ini = (struct Cell*)malloc(sizeof(struct Cell));
                    pont_ini->next = NULL;
                    pont_fim = pont_ini;
                }
                else{
                    pont_fim->next = (struct Cell*)malloc(sizeof(struct Cell));
                    pont_fim = pont_fim->next;
                    pont_fim->next = NULL;
                }
                reset_memory(request,answer);

                printf("Digite quantos numeros deseja: ");
                scanf("%d",&num);

                insert_array(request,num);

                id++;
                generate_head(request,num,id,op);
                save_package_req(request,pont_fim);
                env = send(sockfd,htole64(request), sizeof(struct Request), 0);
                if(env == -1){
                perror("\nError no envio do pacote: ");
                return EXIT_FAILURE;
                }

                rec = recv(sockfd, le64toh(answer), sizeof(struct Answer) , 0);
                if(rec == -1){
                    perror("\nError na entrega do pacote: ");
                    return EXIT_FAILURE;
                }
                show_package_ans(answer);
                save_package_ans(answer,pont_fim);

                free(request);
                request = NULL;
                free(answer);
                answer = NULL;
                break;

            case HISTORY:
                printf("\n-----------------------HISTORY-----------------------\n\n");
                show_history(pont_ini);
                break;

            case FINISH:
                printf("Fim da aplicacao\n");
                checker = FALSE;
                break;
        }
    }

    close(sockfd);
    printf("\nCliente Encerrado! \n");
    return 0;

}
