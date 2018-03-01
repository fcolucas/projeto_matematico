#include <stdio.h>
#include <stdlib.h>
#include "../lib/protocol.h"
#include "../lib/history.h"

void save_package_req(struct request *request, struct history *history){
    history->request = *request;
}

void save_package_ans(struct answer *answer, struct history *history){
    history->answer = *answer;
}

void show_history(struct history *history){
    int i;
    if(history == NULL)
    {
        printf("NÃO FORAM REALIZADAS OPERAÇÕES\n\n");
        return;
    }
    else
    {
        do{
            for(i = 0; i < history->request.head.length/SIZE_DOUBLE - 1 ; i++){
                if(history->request.head.operation == op_add)
                    printf("%.2lf + ",history->request.numbers[i]);

                else if(history->request.head.operation == op_subtract)
                    printf("%.2lf - ",history->request.numbers[i]);

                else if(history->request.head.operation == op_multiply)
                    printf("%.2lf x ",history->request.numbers[i]);

                else if(history->request.head.operation == op_divide)
                    printf("%.2lf / ",history->request.numbers[i]);
            }

            if(history->answer.head.operation == math_sucess){
                printf("%.2lf ",history->request.numbers[i]);
                printf("= %.2lf\n",history->answer.total);
            }

            else if(history->answer.head.operation == math_error || history->answer.head.operation == sintax_error){
                printf("%.2lf ",history->request.numbers[i]);
                printf("= Error na Resposta.\n");
            }

            history = history->next;
        }while(history != NULL);

    }
    printf("\n");
}

struct history *allocate_new_history(struct history *first_history,struct history *last_history){
    if(first_history == NULL){
        first_history = (struct history*)malloc(sizeof(struct history));
        first_history->next = NULL;
        last_history = first_history;
    }

    else{
        last_history->next = (struct history*)malloc(sizeof(struct history));
        last_history = last_history->next;
        last_history->next = NULL;
    }

    return last_history;
}