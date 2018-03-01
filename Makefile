CC = gcc
CFLAGS = -I
OBJ = client.o protocol.o server.o history.o communication.o
LIB_DIR = ./lib
SRC_DIR = src
BIN_DIR = bin

all:	$(OBJ)
	$(CC) client.o protocol.o history.o communication.o -o $(BIN_DIR)/client
	$(CC) server.o protocol.o communication.o -o $(BIN_DIR)/server

client.o: 	$(SRC_DIR)/client.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/client.c $(CFLAGS) $(LIB_DIR) -o client.o

protocol.o:    $(SRC_DIR)/protocol.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/protocol.c $(CFLAGS) $(LIB_DIR) -o protocol.o

server.o:   $(SRC_DIR)/server.c $(LIB_DIR)/protocol.h
	$(CC) -c $(SRC_DIR)/server.c $(CFLAGS) $(LIB_DIR) -o server.o

history.o:	$(SRC_DIR)/history.c $(LIB_DIR)/history.h
	$(CC) -c $(SRC_DIR)/history.c $(CFLAGS) $(LIB_DIR) -o history.o	

communication.o:	$(SRC_DIR)/communication.c $(LIB_DIR)/communication.h
	$(CC) -c $(SRC_DIR)/communication.c $(CFLAGS) $(LIB_DIR) -o communication.o

clean:
	rm *.o
