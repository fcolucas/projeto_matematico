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

void answer_head(struct answer *answer, int length, int id, int op, double total){
    answer->head.type = TYPE_ANSWER;
    answer->head.length = length*SIZE_DOUBLE;
    answer->head.id = id;
    answer->head.operation = op;
    answer->total = total;
}

void operate(struct request *request, struct answer *answer){
    int id = request->head.id;
    int zero = FALSE;
    int len = request->head.length/SIZE_DOUBLE;
    int i;
    double total = request->numbers[0];
	if(len != 1){
    	switch(request->head.operation){
			case OP_ADD:
				for(i = 1; i < len; i++){
				    total += request->numbers[i];
                }
				answer_head(answer, request->head.length, id, MATH_SUCESS, total);
				break;

			case OP_SUBTRACT:
				for(i = 1; i < len; i++){
				    total -= request->numbers[i];
                }
				answer_head(answer, request->head.length, id, MATH_SUCESS, total);
				break;

			case OP_MULTIPLY:
				for(i = 1; i < len; i++){
				    total *= request->numbers[i];
                }
				answer_head(answer, request->head.length, id, MATH_SUCESS, total);
				break;

			case OP_DIVIDE:
				for(i = 1; i < len; i++){
					if(request->numbers[i] == 0){ //caso tenha zero
						zero = TRUE;
						break;
					}
					else total /= request->numbers[i];
				}

				if(zero == FALSE) answer_head(answer, request->head.length, id, MATH_SUCESS, total);
				else answer_head(answer, request->head.length, id, MATH_ERROR, total);
				break;
		}
	}
	
	else answer_head(answer, request->head.length, id, SINTAX_ERROR, total);
}
/*
void reset_memory(struct request *request, struct answer *answer){
   memset(request->head,0,sizeof(request->head));
   memset(request->numbers,0,sizeof(request->numbers));
   memset(answer->head,0,sizeof(answer->head));
   answer->total = 0;
}*/

void show_data(struct request *request, struct answer *answer){
    printf("HEAD Requisição:\n %d (Tipo Requisição) %d (Tamanho Bytes) %d (ID)\n",request->head.type,request->head.length,request->head.id);
    if(answer->head.operation == MATH_SUCESS){
        printf("Tipo da Operação: ");
        switch(request->head.operation){
            case OP_ADD: 
                printf("Adição\n"); 
                break;
            
            case OP_SUBTRACT: 
                printf("Subtração\n"); 
                break;
            
            case OP_MULTIPLY: 
                printf("Multiplicação\n"); 
                break;
            
            case OP_DIVIDE: 
                printf("Divisão\n"); 
                break;
        }
        
        printf("Números: ");
        int len = request->head.length/SIZE_DOUBLE;
        
        for(int j=0; j < len; j++){
            printf("%.2lf ", request->numbers[j]);
        }
        printf("\nResposta: %.2lf \n\n", answer->total);
    }
    
    else if(answer->head.operation == MATH_ERROR) printf("Impossibilidade Matemática\n\n");
    
    else printf("Erro de Sintaxe\n\n");
}



void generate_head(struct request *request, int length,int id, int op){
    request->head.type = TYPE_REQUEST;
    request->head.length = length * SIZE_DOUBLE;
    request->head.id = id;
    request->head.operation = op;
}





void show_package_ans(struct answer *answer){
    if(answer->head.operation == MATH_SUCESS) 
    	printf("\nResposta: %.2lf\n\n",answer->total);
    else if(answer->head.operation == MATH_ERROR) 
    	printf("\nResposta: Impossibilidade Matemática!\n");
    else if (answer->head.operation == SINTAX_ERROR) 
    	printf("\nResposta: Error de Sintaxe!\n");
}

void insert_array(struct request *request, int num){
    double valor;
    int i;
    for(i = 0; i < num ; i++){
        printf("Digite o %d° numero:  ", i+1);
        scanf("%lf",  &valor);
        request->numbers[i] = valor;
    }
}







int submenu(){
    int op;
    printf("          \n");
    printf("0 - Somar \n");
    printf("1 - Subtrair  \n");
    printf("2 - Multiplicar \n");
    printf("3 - Dividir \n");
    printf("4 - Retornar ao Menu\n");
    printf("Selecione a opcao: ");
    scanf("%d", &op);
    return op;
}

int menu(){
	int op;
	printf("(------------CALCULADORA--------------)\n");
	printf("1 - Operações\n");
	printf("2 - Histórico\n");
	printf("0 - Sair\n");
	printf("Selecione a opcao: ");
	scanf("%d", &op);	
    return op;
}

