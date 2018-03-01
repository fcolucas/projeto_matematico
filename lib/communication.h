#ifndef COMMUNICATION_H_INCLUDED
#define COMMUNICATION_H_INCLUDED

#define FLAGS 0
#define SIZE_DOUBLE 8





struct sockaddr_in local;
struct sockaddr_in remote;

void setup_socket(struct sockaddr_in *server, int port, char *ip);

int config_socket(struct sockaddr_in local, struct sockaddr_in remote, int sockfd, int port);

#endif // COMMUNICATION_H_INCLUDED
