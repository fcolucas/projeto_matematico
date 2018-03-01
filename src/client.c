#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <locale.h>
#include "../lib/protocol.h"
#include "../lib/history.h"
#include "../lib/communication.h"


struct sockaddr_in remote;

int main(){
    setlocale(LC_ALL,"pt_BR_utf8");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec;
    int env;
    char id = 0;
    int op;
    char num;
    int checker = TRUE;
    int x[1];
    int *length_array = x;
    char ip[32];
    double array_resp[5];
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
        switch((enum menu)op){

            case menu_operation:
                op = submenu();
                if (op == RETURN_MENU)
                    break;

                last_history = allocate_new_history(first_history,last_history);
                if(first_history == NULL)
                    first_history = last_history;

                //reset_memory(request,answer);

                printf("Digite quantos números deseja: ");
                scanf("%c", &num);

                insert_array(request,num);

                id++;
                request_head(request, num, id, (char) op);
                save_package_req(request,last_history);

                double *array = (double*)malloc(sizeof(double)*(LENGTH_HEAD+num));
                length_array[0] = htole32(LENGTH_HEAD+num);

                convert_struct_array(request, array, LENGTH_HEAD+num, TYPE_REQUEST);

                env = (int) send(sockfd, length_array, sizeof(int), FLAGS);
                env = (int) send(sockfd, array, sizeof(double)*(LENGTH_HEAD+num), FLAGS);

//                env = (int) send(sockfd, htole64(request), sizeof(struct request), FLAGS);
                if(env == -1){
                    perror("\nErro no envio do pacote: ");
                    return EXIT_FAILURE;
                }

                rec = (int) recv(sockfd, le64toh(array_resp), sizeof(array_resp), FLAGS);
                if(rec == -1){
                    perror("\nErro no recebimento do pacote: ");
                    return EXIT_FAILURE;
                }

                convert_array_struct(answer, array_resp);

                show_package_ans(answer);
                save_package_ans(answer,last_history);

                break;

            case history:
                printf("\n-----------------------HISTORY-----------------------\n\n");
                show_history(first_history);
                break;

            case finish:
                printf("Fim da aplicação\n");
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