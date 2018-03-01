#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#define TYPE_REQUEST 0
#define TYPE_ANSWER 1
#define SIZE_DOUBLE 8
#define LENGTH_HEAD 4

#define RETURN_MENU 4

#define TRUE 1
#define FALSE 0
#define PORT 3000
#define FLAGS 0

enum operations{
    op_add, op_subtract, op_multiply, op_divide};

enum menu{
    finish, menu_operation, history};

enum status_operation{
    math_sucess, math_error, sintax_error};

enum arguments{
    type, length, id, operation, total};

struct head{
    char type;
    char length;
    char id;
    char operation;
};

struct request{
    struct head head;
    double *numbers;
};

struct answer{
    struct head head;
    double total;
};

void answer_head(struct answer *answer, char length, char id, char op, double total);

void request_head(struct request *request, char length, char id, char op);

void operate(struct request *request, struct answer *answer);

//void reset_memory(struct request *request, struct answer *answer);

void convert_struct_array(void *arg, double *arr, int tam, int flag);

void convert_array_struct(void *arg,double *arr);

void show_data(struct request *request, struct answer *answer);

void show_package_ans(struct answer *answer);

void insert_array(struct request *request, int num);

int menu();

int submenu();

#endif // PROTOCOL_H_INCLUDED