#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#define TYPE_REQUEST 0
#define TYPE_ANSWER 1
#define SIZE_DOUBLE 8

#define OP_ADD 0
#define OP_SUBTRACT 1
#define OP_MULTIPLY 2
#define OP_DIVIDE 3
#define MENU_OPERATION 1
#define RETURN_MENU 4
#define HISTORY 2
#define FINISH 0

#define MATH_SUCESS 0
#define MATH_ERROR 1
#define SINTAX_ERROR 2

#define TRUE 1
#define FALSE 0
#define PORT 3000
#define FLAGS 0


struct head{
	char type;
    char length;
    char id;
    char operation;
};

struct request{
	struct head head;
	double numbers[20];
};

struct answer{
	struct head head;
	double total;
};





void answer_head(struct answer *answer, int length, int id, int op, double total);

void request_head(struct request *request, int length, int id, int op);

void operate(struct request *request, struct answer *answer);

//void reset_memory(struct request *request, struct answer *answer);

void show_data(struct request *request, struct answer *answer);






void show_package_ans(struct answer *answer);

void generate_head(struct request *pc, int length, int id, int p);

void insert_array(struct request *req, int num);


int menu();

int submenu();

#endif // PROTOCOL_H_INCLUDED
