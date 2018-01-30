#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

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

void answer_head(struct answer *ans, int length, int id, int op, int total);

void request_head(struct request *req, int length, int id, int op);

void operation(struct request *req, struct answer *ans);



#endif // PROTOCOL_H_INCLUDED
