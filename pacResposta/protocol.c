#include "protocol.h"

void answer_head(struct answer *ans, int length, int id, int op, double total){
    ans->head[TYPE] = TYPE_ANWSER;
    ans->head[LENGTH] = length*sizeof(double); /*Tamanho do pacote*/
    ans->head[ID] = id; /*Identificador da operação(o mesmo do pacote requisição)*/
    ans->head[OPERATION] = op; /*Status da operaçao*/
    ans->total = total;
}

void operation(struct request *request, struct answer *answer){
    int id = request->head[ID], zero = FALSE, len = request->head[LENGTH]/8 /*qnte de numeros*/, i;
    double total = request->numeros[0];
	if(len != 1){
    	switch(request->head[OPERATION]){
		    case OP_ADD: /*Operação de adição*/
		        for(i=1; i<len; i++){
		                total += request->numeros[i];
		            }
                answer_head(ans, request->head[LENGTH], id, MATH_SUCESS, total);
                break;

		    case OP_SUBTRACT: /*Operação de subtração*/
		        for(i=1; i<len; i++){
		                total -= request->numeros[i];
                    }
                answer_head(ans, request->head[LENGTH], id, MATH_SUCESS, total);
                break;

		    case OP_MULTIPLY: /*Operação de multiplicação*/
		        for(i=1; i<len; i++){
		                total *= request->numeros[i];
		            }
		            answer_head(ans, request->head[LENGTH], id, MATH_SUCESS, total);
		            break;

		    case OP_DIVIDE: /*Operação de divisão*/
				for(i=1; i<len; i++){
					if(request->numeros[i] == 0){ //caso tenha zero
						zero = TRUE;
						break;
					}
					else{
		                total /= request->numeros[i];
					}
				}

                if(zero == FALSE){
                    answer_head(ans, request->head[LENGTH], id, MATH_SUCESS, total);
                }else{
		            answer_head(ans, request->head[LENGTH], id, MATH_ERROR, total);
                }
                break;
		}
	}else answer_head(ans, request->head[LENGTH], id, SINTAX_ERROR, total);

void reset_memory(struct Request *request, struct Answer *answer){
   memset(request->head,0,sizeof(request->head));
   memset(request->numeros,0,sizeof(request->numeros));
   memset(answer->head,0,sizeof(answer->head));
   answer->total = 0;
}

void show_data(struct Request *request, struct Answer *answer){
    printf("HEAD Requisição:\n %d (Tipo Requisição) %d (Tamanho Bytes) %d (ID)\n",request->head[TYPE],request->head[LENGTH],request->head[ID]);
    if(answer->head[OPERATION] == MATH_SUCESS){
        printf("Tipo da Operação: ");
        switch(request->head[OPERATION]){
            case OP_ADD: printf("Adição\n"); break;
            case OP_SUBTRACT: printf("Subtração\n"); break;
            case OP_MULTIPLY: printf("Multiplicação\n"); break;
            case OP_DIVIDE: printf("Divisão\n"); break;
        }
        printf("Números: ");
        int len = request->head[LENGTH]/8;
        for(int j=0; j<len; j++)
            printf("%.2lf ", request->numeros[j]);
        printf("\nResposta: %.2lf \n\n", answer->total);
    }
    else if(answer->head[OPERATION] == MATH_ERROR) printf("Impossibilidade Matemática\n\n");
    else printf("Erro de Sintaxe\n\n");
    printf("Aguardando novo pacote...\n");
}
