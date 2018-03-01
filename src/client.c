#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <endian.h>
#include <locale.h>
#include "protocol.h"
#include "history.h"
#include "communication.h"


struct sockaddr_in remote;

int main(){
    setlocale(LC_ALL,"pt_BR_utf8");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec;
    int env;
    int id = 0;
    int op;
    int num;
    int checker = TRUE;
    double valor;
    char ip[32];
    struct history *first_history = NULL, *last_history = NULL;
    struct request *request = (struct request *)malloc(sizeof(struct request));
    struct answer *answer = (struct answer *)malloc(sizeof(struct answer));
    socklen_t len = sizeof(remote);

    printf("Digite o Ip desejado para conexao: ");
    fgets(ip,14,stdin);// 192.168.1.120-> lucas

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("\nSocket criado!\n");
    }

    setup_socket(&remote,PORT,ip);

    if(connect(sockfd, (struct sockaddr *)&remote, len) < 0){
        perror("connect ");
        exit(1);
    }

    printf("Conectado!\n\n");

    while(checker){

        op = menu();
        switch(op){

            case MENU_OPERATION:
                op = submenu();
                if (op == RETURN_MENU)
                    break;
                
                last_history = allocate_new_history(first_history,last_history);
                if(first_history == NULL)
                    first_history = last_history;

                //reset_memory(request,answer);

                printf("Digite quantos numeros deseja: ");
                scanf("%d",&num);

                insert_array(request,num);

                id++;
                generate_head(request,num,id,op);
                save_package_req(request,last_history);
                env = send(sockfd,htole64(request), sizeof(struct request), FLAGS);
                if(env == -1){
                    perror("\nError no envio do pacote: ");
                    return EXIT_FAILURE;
                }

                rec = recv(sockfd, le64toh(answer), sizeof(struct answer) , FLAGS);
                if(rec == -1){
                    perror("\nError na entrega do pacote: ");
                    return EXIT_FAILURE;
                }
                show_package_ans(answer);
                save_package_ans(answer,last_history);

                break;

            case HISTORY:
                printf("\n-----------------------HISTORY-----------------------\n\n");
                show_history(first_history);
                break;

            case FINISH:
                printf("Fim da aplicacao\n");
                checker = FALSE;
                break; 
            default:
                printf("Opção Inválida! \n");
                break;
        }
            
    }

    close(sockfd);
    printf("\nCliente Encerrado! \n");
    return 0;

}