#ifndef PROTOCOL_H
#define PROTOCOL_H

#define TYPE_REQUEST 0
#define TYPE_ANWSER 1

#define TYPE 0
#define LENGTH 1
#define ID 2
#define OPERATION 3

#define OP_ADD 0
#define OP_SUBTRACT 1
#define OP_MULTIPLY 2
#define OP_DIVIDE 3

#define MATH_SUCESS 0
#define MATH_ERROR 1
#define SINTAX_ERROR 2

#define TRUE 1
#define FALSE 0
#define PORT 3000
#define FLAGS 0

struct sockaddr_in local;
struct sockaddr_in remote;

struct Request{
	char head[4];
	double numeros[20];
};

struct Answer{
	char head[4];
	double total;
};

int config_socket(struct sockaddr_in local, struct sockaddr_in remote, int sockfd, int port);

void answer_head(struct Answer *answer, int length, int id, int op, double total);

void request_head(struct Request *request, int length, int id, int op);

void operation(struct Request *request, struct Answer *answer);

void reset_memory(struct Request *request, struct Answer *answer);

void show_data(struct Request *request, struct Answer *answer);

#endif // PROTOCOL_H_INCLUDED
