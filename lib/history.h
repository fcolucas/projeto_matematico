#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED

#include "protocol.h"
struct  history
{
    struct request request;
    struct answer answer;
    struct history *next;
};

void save_package_req(struct request *request, struct history *history);

void save_package_ans(struct answer *answer, struct history *history);

struct history *allocate_new_history(struct history *first_history, struct history *last_history);

void show_history(struct history *history);


#endif // HISTORY_H_INCLUDED