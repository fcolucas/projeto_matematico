//ANDERSON
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <endian.h>
#include <time.h>



#define PORTA 4000


struct sockaddr_in remoto;

struct pacote{
    char head [4];
    double numeros[20];
};

struct pacoteResposta{
    char head[4];
    double resp;
};

struct historico{ // estrutura para guardar os pacotes de requisiçao e resposta
    struct pacote req[100]; // requisiçoes
    struct pacoteResposta resp[100]; // respostas
    char nreq; // numero de requisoções no array req
    char nresp; // numero de respostas no array resp
};

int menu()
{
    int op;
    printf("(------------CALCULADORA--------------)\n");
    printf("1 - Somar \n");
    printf("2 - Subtrair  \n");
    printf("3 - Multiplicar \n");
    printf("4 - Dividir \n");
    printf("5 - Historico\n");
    printf("0 - Sair da aplicação\n");
    printf("Selecione a operação: ");
    scanf("%d", &op);
    return op;
}

void gera_HEAD(struct pacote *pc, int length,int ID, int Op) // recebe ponteiros para o pacote o tamanho do pacote o ID e para a OP
{
    //srand((unsigned) time(NULL));
    //rand() % 255
    pc->head[0] = 0;
    pc->head[1] = length * 8;
    pc->head[2] = ID;
    pc->head[3] = Op;
}

void salvar_pacote_resq(struct pacote *pc,struct historico *hist) //recebe um ponteiro para o pacote de requisiçao e um ponteiro para a estrutura de historico
{
    if(hist->nreq <= 99) // se valor o valor de estruturas salvas atingir 100 nada mais ser salvo
    {
        hist->req[hist->nreq] = *pc;
        hist->nreq += 1;
        //hist->nreq  = hist->nreq +1 ; (assim da certo se isso noa funcionar
    }
    else
        printf("\n Memória cheia! Nao é possível salvar a operação para consulta \n");

}

void salvar_pacote_resp(struct pacoteResposta *pacResp, struct historico *hist) //recebe um ponteiro para o pacote de resposta e um ponteiro para a estrutura onde será salva
{
    if(hist->nresp <= 99) // se valor o valor de estruturas salvas atingir 100 nada mais ser salvo
    {
        hist->resp[hist->nresp] = *pacResp;
        hist->nresp += 1;

    }
    else
        printf("\n Memória cheia! Nao é possível salvar a resposta para consulta \n");

}

void mostra_pacote_resp(struct pacoteResposta *PacResp, struct historico *hist) // mostra a resposta enviada e salva somente se a reposta for bem sucedida
{
    if( PacResp->head[3] == 0)
    {
        //printf("gerador de HEAD %d (requisicao) %d (tamanho) %d (ID) %d (OP)\n",PacResp->head[0],PacResp->head[1],PacResp->head[2],PacResp->head[3]);
        printf("\nResposta: %.2lf\n\n",PacResp->resp);
        salvar_pacote_resp(PacResp,hist);//salvando resposta
        /*VERIFICAR SE ESSA ESTRUTA DE SALVAR ESTA REALMENTE FUNCIONANDO*/
    }
    else if(PacResp->head[3] == 1)
    {
        printf("\nResposta: Impossibilidade Matemática!\n");
    }
    else if (PacResp->head[3] == 2)
    {
        printf("\nResposta: Error de Sintaxe!\n");
    }

}


void preenche_array(struct pacote *Pac, int num)// recebe um ponteiro para o pacote de requisicao e a quantidade de numeros a serem inseridos
{
    double valor;
     for(int i = 0; i<num;i++)
    {
        printf("Digite o %d° numero:  ",i+1);
        scanf("%lf",&valor);
        Pac->numeros[i] = valor;
    }
}

void mostrar_historico(struct historico *hist)
{
    int verificador,i,j,k;
    for( i = 0; i<hist->nreq;i++) // acessa os elementos de req
    {
        verificador = 0;
        if(hist->req[i].head[3] == 0) // confere o tipo de operação requisitada, soma no caso
        {
            for( j = 0;j<hist->nresp;j++) // percorre o array de respostas atrás no resultado
            {
                if(hist->req[i].head[2] == hist->resp[j].head[2]) // enconta a resposta pelo ID do pacote
                {

                    for( k = 0; k<hist->req[i].head[1]/8 - 1;k++) // printar até penultimo numero
                    {
                        printf("%.2lf+",hist->req[i].numeros[k]);
                    }
                    printf("%.2lf = ",hist->req[i].numeros[k]); // ultimo numero printado
                    printf("%.2lf\n\n",hist->resp[j].resp); // printa resposta
                    verificador = 1;
                    break;
                }

            }

            if(verificador == 0)// caso o resultado nao seja encontrado é printado um sinal de error
            {
                printf("\n nao ha resposta para equaçao \n");
                for( k = 0;i<hist->req[i].head[1]/8-1;k++)
                {
                    printf("%.2lf+",hist->req[i].numeros[k]);
                }
                printf("%.2lf = ",hist->req[i].numeros[k]);
                printf("Error na resposta!\n\n");

            }

        }

        else if(hist->req[i].head[3] == 1) // conferindo operação (subtração)
        {
            for( j = 0;j<hist->nresp;j++) // percorre o array de respostas atrás no resultado
            {
                if(hist->req[i].head[2] == hist->resp[j].head[2]) // enconta a resposta pelo ID do pacote
                {
                    for( k = 0; k<hist->req[i].head[1]/8 -1;k++) // printar até penultimo numero
                    {
                        printf("%.2lf-",hist->req[i].numeros[k]);
                    }
                    printf("%.2lf = ",hist->req[i].numeros[k]); // ultimo numero printado
                    printf("%.2lf\n\n",hist->resp[j].resp); // printa resposta
                    verificador = 1;
                }

            }

            if(verificador == 0)// caso o resultado nao seja encontrado é printado um sinal de error
            {
                for( k = 0;i<hist->req[i].head[1]/8-1;k++)
                {
                    printf("%.2lf-",hist->req[i].numeros[k]);
                }
                printf("%.2lf = ",hist->req[i].numeros[k]);
                printf("Error na resposta!\n\n");

            }
        }

        else if(hist->req[i].head[3] == 2) //conferindo operação (multipliação)
        {
            for( j = 0;j<hist->nresp;j++) // percorre o array de respostas atrás no resultado
            {
                if(hist->req[i].head[2] == hist->resp[j].head[2]) // enconta a resposta pelo ID do pacote
                {
                    for( k = 0; k<hist->req[i].head[1]/8 -1;k++) // printar até penultimo numero
                    {
                        printf("%.2lfx",hist->req[i].numeros[k]);
                    }
                    printf("%.2lf = ",hist->req[i].numeros[k]); // ultimo numero printado
                    printf("%.2lf\n\n",hist->resp[j].resp); // printa resposta
                    verificador = 1;
                    break; // parando o for
                }

            }

            if(verificador == 0)// caso o resultado nao seja encontrado é printado um sinal de error
            {
                for( k = 0;i<hist->req[i].head[1]/8 -1;k++)
                {
                    printf("%.2lfx",hist->req[i].numeros[k]);
                }
                printf("%.2lf = ",hist->req[i].numeros[k]);
                printf("Error na resposta!\n\n");

            }
        }

        else if(hist->req[i].head[3] == 3) // conferindo operação (divisão)
        {
            for( j = 0;j<hist->nresp;j++) // percorre o array de respostas atrás no resultado
            {
                if(hist->req[i].head[2] == hist->resp[j].head[2]) // enconta a resposta pelo ID do pacote
                {
                    for( k = 0; k<hist->req[i].head[1]/8 -1;k++) // printar até penultimo numero
                    {
                        printf("%.2lf/",hist->req[i].numeros[k]);
                    }
                    printf("%.2lf = ",hist->req[i].numeros[k]); // ultimo numero printado
                    printf("%.2lf\n\n",hist->resp[j].resp); // printa resposta
                    verificador = 1;
                }

            }

            if(verificador == 0)// caso o resultado nao seja encontrado é printado um sinal de error
            {
                for( k = 0;i<hist->req[i].head[1]/8 -1;k++)
                {
                    printf("%.2lf/",hist->req[i].numeros[k]);
                }
                printf("%.2lf = ",hist->req[i].numeros[k]);
                printf("Error na resposta!\n\n");

            }
        }
    }
}

void config_socket(struct sockaddr_in *server, int porta,char *ip)
{
    server->sin_family = AF_INET;
    server->sin_port = htons(porta);
    inet_pton(AF_INET, ip, &remoto.sin_addr);
    memset(server->sin_zero,0x0,8);
}

void zerar_memoria(struct pacote *Pac, struct pacoteResposta *PacResp)
{
    memset(Pac->head,0,sizeof(Pac->head));
    memset(Pac->numeros,0,sizeof(Pac->numeros));
    memset(PacResp->head,0,sizeof(PacResp->head));
    PacResp->resp = 0;
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int rec, env, ID = 0, op, num;
    double valor;
    char ip[32];
    socklen_t len = sizeof(remoto);
    strcpy(ip, "192.168.1.120"); // 192.168.1.120-> lucas
    struct historico *hist = (struct historico*)malloc(sizeof(struct historico)); // alocando o historico
    hist->nreq = 0;
    hist->nresp = 0;    //atribuindo valor a para o hist.

    if(sockfd == -1){
        perror("socket ");
        exit(1);
    }else{
        printf("Socket criado!\n");
    }

    config_socket(&remoto,PORTA,ip);
    /*
    remoto.sin_family = AF_INET;
    remoto.sin_port = htons(PORTA);
	inet_pton(AF_INET, ip, &remoto.sin_addr);
    memset(remoto.sin_zero, 0x0, 8);
    */
    if(connect(sockfd, (struct sockaddr *)&remoto, len) < 0){
        perror("connect ");
        exit(1);
    }

    printf("Conectado!\n\n");




    while(1)
    {
        op = menu();
        if(op == 0)
        {
            break;
        }
        if(op == 1 || op == 2 || op == 3 || op == 4)
        {
            struct pacote *Pac = (struct pacote*)malloc(sizeof(struct pacote));// alocando pacote requisiçao
            struct pacoteResposta *PacResp = (struct pacoteResposta*)malloc(sizeof(struct pacoteResposta));//alocando pacote resposta
            zerar_memoria(Pac,PacResp); // resolvendo problemas de lixo na memoria

            printf("Digite quantos numeros deseja: ");
            scanf("%d",&num);

            preenche_array(Pac,num); // preenche o array com os numeros desejados

            ID++; // resolvendo problemas no ID
            gera_HEAD(Pac,num,ID,op-1);
            salvar_pacote_resq(Pac,hist);// salvando pacote de requisiçao
            env = send(sockfd,htole64(Pac), sizeof(struct pacote), 0); // enviando pacote de requisicao

            if(env == -1){// verificando envio do pacote
                perror("\nErro no envio do pacote: ");
                return EXIT_FAILURE;
            }

            rec = recv(sockfd, le64toh(PacResp), sizeof(struct pacoteResposta) , 0); // recebendo pacote
            if(rec == -1){ // verificando entrega do pacote
                perror("\nErro na entrega do pacote: ");
                return EXIT_FAILURE;
            }

            mostra_pacote_resp(PacResp,hist); // mostrando resultado e salvando pacote se a operaçao for bem sucedida



            free(Pac); Pac = NULL;
            free(PacResp); PacResp = NULL;       // liberando memoria alocada (Fim da operaçao)




            /*TESTAR PACOTE DEPOIS DO FREE*/
        }
        else if (op == 5)
        {
            printf("\n--------------HISTORICO DE OPERACOES--------------\n");
            mostrar_historico(hist);
        }
        else
        {
            printf("\n Opção Inválida!\n ");
        }
    }



    close(sockfd);
    printf("\nCliente Encerrado! \n");
    return 0;
}
