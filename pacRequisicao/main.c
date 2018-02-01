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

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec, env, id = 0, op, num, ver = 0;
    double valor;
    char ip[32];
    struct Cell *pont_ini = NULL, *pont_fim = NULL;
    struct Request *pac;
    struct Answer *pac_resp;

    socklen_t len = sizeof(remote);
    strcpy(ip, "192.168.1.120"); // 192.168.1.120-> lucas

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("Socket criado!\n");
    }

    setup_socket(&remote,PORTA,ip);

    /*
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORTA);
	inet_pton(AF_INET, ip, &remote.sin_addr);
    memset(remote.sin_zero, 0x0, 8);
    */
    if(connect(sockfd, (struct sockaddr *)&remote, len) < 0){
        perror("connect ");
        exit(1);
    }

    printf("Conectado!\n\n");




    while(TRUE)
    {
        op = menu();
        switch(op)
        {
            case OP_ADD:
            case OP_SUBTRACT:
            case OP_MULTIPLY:
            case OP_DIVIDE:
                pac = (struct Request *)malloc(sizeof(struct Request));
                pac_resp = (struct Answer *)malloc(sizeof(struct Answer));
                if(pont_ini == NULL)
                {
                    pont_ini = (struct Cell*)malloc(sizeof(struct Cell));
                    pont_ini->next = NULL;
                    pont_fim = pont_ini;
                }
                pont_fim->next = (struct Cell*)malloc(sizeof(struct Cell));
                pont_fim = pont_fim->next;
                pont_fim->next = NULL;



                reset_memory(pac,pac_resp);

                printf("Digite quantos numeros deseja: ");
                scanf("%d",&num);

                insert_array(pac,num);

                id++;
                generate_head(pac,num,id,op);
                save_package_req(pac,pont_fim);
                env = send(sockfd,pac, sizeof(struct Request), 0);
                if(env == -1){
                perror("\nError no envio do pacote: ");
                return EXIT_FAILURE;
                }

                rec = recv(sockfd, le64toh(pac_resp), sizeof(struct Answer) , 0);
                if(rec == -1){
                    perror("\nError na entrega do pacote: ");
                    return EXIT_FAILURE;
                }
                show_package_ans(pac_resp);
                save_package_ans(pac_resp,pont_fim);

                free(pac); pac = NULL;
                free(pac_resp); pac_resp = NULL;
                break;

            case HISTORY:
                printf("\n-----------------------HISTORY-----------------------\n\n");
                show_history(pont_ini);

            case FINISH:
                printf("Fim da aplicacao\n");
                ver = TRUE;
                break;



        }
        if(ver)
            break;

    }

    close(sockfd);
    printf("\nCliente Encerrado! \n");
    return 0;

}
