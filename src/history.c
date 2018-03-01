#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "protocol.h"
#include "history.h"

void save_package_req(struct request *request, struct history *ht){
    ht->request = *request;
}

void save_package_ans(struct answer *answer, struct history *ht){
    ht->answer = *answer;
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
            if(history->request.head.operation == OP_ADD) 
            	printf("%.2lf + ",history->request.numbers[i]);
            
            else if(history->request.head.operation == OP_SUBTRACT) 
            	printf("%.2lf - ",history->request.numbers[i]);
            
            else if(history->request.head.operation == OP_MULTIPLY) 
            	printf("%.2lf x ",history->request.numbers[i]);
            
            else if(history->request.head.operation == OP_DIVIDE) 
            	printf("%.2lf / ",history->request.numbers[i]);
        }
        
        if(history->answer.head.operation == MATH_SUCESS){
            printf("%.2lf ",history->request.numbers[i]);
            printf("= %.2lf\n",history->answer.total);
        }
        
        else if(history->answer.head.operation == MATH_ERROR || history->answer.head.operation == SINTAX_ERROR){
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