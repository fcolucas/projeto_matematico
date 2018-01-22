#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <endian.h>

#define PORTA 2000
#define TAM 256
#define TRUE 1
#define FALSE 0

struct sockaddr_in local;
struct sockaddr_in remoto;

struct pacote{
	char head[4];
	double numeros[20];
};

struct resposta{
	char head[4];
	double resposta;
};

void gera_RESP(struct resposta *resp, int length, int id, int Op, int total){
    resp->head[0] = 1; /*Tipo de pacote*/
    resp->head[1] = length; /*Tamanho do pacote*/
    resp->head[2] = id; /*Identificador da operação(o mesmo do pacote requisição)*/
    resp->head[3] = Op; /*Status da operaçao*/
    resp->resposta = total;
}

void operacao(struct pacote *pac, struct resposta *resp){
    int Id = pac->head[2];
    double total = pac->numeros[0];
    int i=0;
    switch(pac->head[3]){
        case 0: /*Operação de adição*/
            for(i=1; i<pac->head[1]; i++){
                    total += pac->numeros[i];
                }
                gera_RESP(resp, pac->head[1], Id, 0, total);
                break;
        case 1: /*Operação de subtração*/
            for(i=1; i<pac->head[1]; i++){
                    total -= pac->numeros[i];
                }
                gera_RESP(resp, pac->head[1], Id, 0, total);
                break;
        case 2: /*Operação de multiplicação*/
            for(i=1; i<pac->head[1]; i++){
                    total *= pac->numeros[i];
                }
                gera_RESP(resp, pac->head[1], Id, 0, total);
                break;
        case 3: /*Operação de divisão*/
            for(i=1; i<pac->head[1]; i++){
                    total /= pac->numeros[i];
                }
                gera_RESP(resp, pac->head[1], Id, 0, total);
                break;
    }
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof(remoto);
    int client, rec, env, j;

    /*Conexão com o socket requisição*/
    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("Socket criado!\n");
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(PORTA);
    memset(local.sin_zero, 0x0, 8);

    if (bind(sockfd, (struct sockaddr *)&local, sizeof(local))==-1){
        perror("bind ");
        exit(1);
    }

    if(listen(sockfd, 1)==-1){
		perror("listen ");
		exit(1);
	}

	printf("Aguardando requisição...\n");

    client = accept(sockfd, (struct sockaddr *)&remoto, &len);
    if(client==-1){
        perror("accept ");
        exit(1);
    }
    printf("Requisição conectado!\n");

    while(1){
        struct pacote *pac = (struct pacote*)malloc(sizeof(struct pacote));
        struct resposta *resp = (struct resposta*)malloc(sizeof(struct resposta));

        rec = recv(client, pac, sizeof(struct pacote), 0);
        printf("Recebido!\n");
        if(rec == 168){ /*rec > 0*/
            printf("Recebido!\n");
            printf("Gerador de HEAD %d (requisicao) %d (tamanho) %d (ID) %d (OP)\n",pac->head[0],pac->head[1],pac->head[2],pac->head[3]);
            for(j=0; j<pac->head[1]; j++){
                printf("%lf ", pac->numeros[j]);
            }
            operacao(pac, resp);
            env = send(client, resp, sizeof(struct resposta), 0);
            if(env > 0) printf("Resposta; %lf\n\n", resp->resposta);
            free(pac);
            free(resp);
        }
    }
    close(client);
    close(sockfd);
    return 0;
}
