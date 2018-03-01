#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include "../lib/protocol.h"

void answer_head(struct answer *answer, char length, char id, char op, double total){
    answer->head.type = TYPE_ANSWER;
    answer->head.length = (char) (length * SIZE_DOUBLE);
    answer->head.id = id;
    answer->head.operation = (enum operations) op;
    answer->total = total;
}

void operate(struct request *request, struct answer *answer){
    char id = request->head.id;
    int zero = FALSE;
    int len = request->head.length/SIZE_DOUBLE;
    int i;
    double total = request->numbers[0];
    if(len != 1){
        switch((enum operations) request->head.operation){
            case op_add:
                for(i = 1; i < len; i++){
                    total += request->numbers[i];
                }
                answer_head(answer, request->head.length, id, math_sucess, total);
                break;

            case op_subtract:
                for(i = 1; i < len; i++){
                    total -= request->numbers[i];
                }
                answer_head(answer, request->head.length, id, math_sucess, total);
                break;

            case op_multiply:
                for(i = 1; i < len; i++){
                    total *= request->numbers[i];
                }
                answer_head(answer, request->head.length, id, math_sucess, total);
                break;

            case op_divide:
                for(i = 1; i < len; i++){
                    if(request->numbers[i] == 0){ //caso tenha zero
                        zero = TRUE;
                        break;
                    }
                    else total /= request->numbers[i];
                }

                if(zero == FALSE) answer_head(answer, request->head.length, id, math_sucess, total);
                else answer_head(answer, request->head.length, id, math_error, total);
                break;
            default:break;
        }
    }

    else answer_head(answer, request->head.length, id, sintax_error, total);
}
/*
void reset_memory(struct request *request, struct answer *answer){
   memset(request->head,0,sizeof(request->head));
   memset(request->numbers,0,sizeof(request->numbers));
   memset(answer->head,0,sizeof(answer->head));
   answer->total = 0;
}*/

void convert_struct_array(void *arg, double *arr, int tam, int flag){

    if(flag == 0){
        struct request *request = (struct request*) arg;
        int i,k = 0;
        arr[type] = htole64(request->head.type);
        arr[length] = htole64(request->head.length);
        arr[id] = htole64(request->head.id);
        arr[operation] = htole64(request->head.operation);
        for(i = 4;i<tam;i++) {
            arr[i] = htole64(request->numbers[k]);
            k += 1;
        }
    }
    else if(flag == 1){
        struct answer *answer = (struct answer*) arg;
        arr[type] = htole64(answer->head.type);
        arr[length] = htole64(answer->head.length);
        arr[id] = htole64(answer->head.id);
        arr[operation] = htole64(answer->head.operation);
        arr[total] = htole64(answer->total);
    }
}

void convert_array_struct(void *arg, double *arr){
    int i, k = 0;
    if(arr[type] == 1){
        struct answer *answer = (struct answer *) arg;
        answer->head.type = (char) le64toh(arr[type]);
        answer->head.length = (char) le64toh(arr[length]);
        answer->head.id = (char) le64toh(arr[id]);
        answer->head.operation = (char) le64toh(arr[operation]);
        answer->total = le64toh(arr[total]);
    }
    else if(arr[type] == 0 ){
        struct request *request = (struct request *) arg;
        request->numbers = (double*)malloc(sizeof(arr[length]));
        request->head.type = (char) le64toh(arr[type]);
        request->head.length = (char) le64toh(arr[length]);
        request->head.id = (char) le64toh(arr[id]);
        request->head.operation = (char) le64toh(arr[operation]);
        for(i = 4; i<(request->head.length)/SIZE_DOUBLE ;i++){
            request->numbers[k] = le64toh(arr[i]);
            k += 1;
        }
    }
}

void show_data(struct request *request, struct answer *answer){
    printf("HEAD Requisição:\n %d (Tipo Requisição) %d (Tamanho Bytes) %d (ID)\n",request->head.type,request->head.length,request->head.id);
    if(answer->head.operation == math_sucess){
        printf("Tipo da Operação: ");
        switch((enum operations) request->head.operation){
            case op_add:
                printf("Adição\n");
                break;

            case op_subtract:
                printf("Subtração\n");
                break;

            case op_multiply:
                printf("Multiplicação\n");
                break;

            case op_divide:
                printf("Divisão\n");
                break;
            default:break;
        }

        printf("Números: ");
        int len = request->head.length/SIZE_DOUBLE;

        for(int j=0; j < len; j++){
            printf("%.2lf ", request->numbers[j]);
        }
        printf("\nResposta: %.2lf \n\n", answer->total);
    }

    else if(answer->head.operation == math_error) printf("Impossibilidade Matemática\n\n");

    else printf("Erro de Sintaxe\n\n");
}

void request_head(struct request *request, char length, char id, char op){
    request->head.type = TYPE_REQUEST;
    request->head.length = (char) (length * SIZE_DOUBLE);
    request->head.id = id;
    request->head.operation = (enum operations) op;
}

void show_package_ans(struct answer *answer){
    if(answer->head.operation == math_sucess)
        printf("\nResposta: %.2lf\n\n",answer->total);
    else if(answer->head.operation == math_error)
        printf("\nResposta: Impossibilidade Matemática!\n");
    else if (answer->head.operation == sintax_error)
        printf("\nResposta: Erro de Sintaxe!\n");
}


void insert_array(struct request *request, int num) {
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
