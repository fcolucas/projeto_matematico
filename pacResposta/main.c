#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTA 4000
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
    resp->head[1] = length*8; /*Tamanho do pacote*/
    resp->head[2] = id; /*Identificador da operação(o mesmo do pacote requisição)*/
    resp->head[3] = Op; /*Status da operaçao*/
    resp->resposta = total;
}

void operacao(struct pacote *pac, struct resposta *resp){
    int Id = pac->head[2], zero = FALSE, len = pac->head[1]/8 /*qnte de numeros*/, i=0;
    double total = pac->numeros[0];
	if(len != 1){
    	switch(pac->head[3]){
		    case 0: /*Operação de adição*/
		        for(i=1; i<len; i++){
		                total = total + pac->numeros[i];
		            }
		            gera_RESP(resp, pac->head[1], Id, 0, total);
		            break;
		    case 1: /*Operação de subtração*/
		        for(i=1; i<len; i++){
		                total -= pac->numeros[i];
                    }
		            gera_RESP(resp, pac->head[1], Id, 0, total);
		            break;
		    case 2: /*Operação de multiplicação*/
		        for(i=1; i<len; i++){
		                total *= pac->numeros[i];
		            }
		            gera_RESP(resp, pac->head[1], Id, 0, total);
		            break;
		    case 3: /*Operação de divisão*/           
				for(i=1; i<len; i++){
					if(pac->numeros[i] == 0){ //caso tenha zero
						zero = TRUE;
						break;
					}else{
		                total /= pac->numeros[i];
					}
				}
					if(zero == FALSE){
		            	gera_RESP(resp, pac->head[1], Id, 0, total);
					}else{
						gera_RESP(resp, pac->head[1], Id, 1, 0); //Impossibilidade Matemática
					}
				break;
		}
	}else gera_RESP(resp, pac->head[1], Id, 2, 0); //Erro de sintaxe, apenas um número!
}

void zerar_memoria(struct pacote *pac, struct resposta *resp){
   memset(pac->head,0,sizeof(pac->head));
   memset(pac->numeros,0,sizeof(pac->numeros));
   memset(resp->head,0,sizeof(resp->head));
   resp->resposta = 0;
}

void mostra_dados(struct pacote *pac, struct resposta *resp){
    int op = resp->head[3]; //Status da operação
    printf("HEAD Requisição:\n %d (Tipo Requisição) %d (Tamanho Bytes) %d (ID)\n",pac->head[0],pac->head[1],pac->head[2]); 
    if(op == 0){
        printf("Tipo da Operação: ");
        switch(pac->head[3]){
            case 0: printf("Adição\n"); break;
            case 1: printf("Subtração\n"); break;
            case 2: printf("Multiplicação\n"); break;
            case 3: printf("Divisão\n"); break;
        }
        printf("Números: ");
        int len = pac->head[1]/8;
        for(int j=0; j<len; j++)
            printf("%.2lf ", pac->numeros[j]);
        printf("\nResposta: %.2lf \n\n", resp->resposta);
    }
    else if(op == 1) printf("Impossibilidade Matemática\n\n");
    else printf("Erro de Sintaxe\n\n");
    printf("Aguardando novo pacote...\n");
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
    printf("Requisição conectado! Aguardando pacote...\n");

    while(1){
        struct pacote *pac = (struct pacote*)malloc(sizeof(struct pacote));
        struct resposta *resp = (struct resposta*)malloc(sizeof(struct resposta));
		
		zerar_memoria(pac, resp);

        rec = recv(client, pac, sizeof(struct pacote), 0);
        //printf("Tamanho RECV: %d Tamanho HEAD: %ld Tamanho Numeros: %ld\n\n", rec, sizeof(pac->head), sizeof(pac->numeros));
        if(rec > 0){
            printf("Pacote Recebido!\n\n");
            operacao(pac, resp);
            env = send(client, resp, sizeof(struct resposta), 0);
            mostra_dados(pac, resp);
            free(pac);  pac = NULL;
            free(resp); resp = NULL;
        }
		else break;
    }
    printf("Encerrando protocolo!! \n");
    close(client);
    close(sockfd);
    return 0;
}
