answer:	package_answer.o protocol.o
	gcc -o answer package_answer.o protocol.o 
	
package_answer.o:	package_answer.c
	gcc -c package_answer.c -o package_answer.o
	
request:	package_request.c
	gcc package_request.c -o request 
	
package_request.o:	package_request.c
	gcc -c package_request.c -o package_request.o
	
protocol.o:	protocol.c protocol.h
	gcc -c protocol.c -o protocol.o
	
clean:
	rm *.o
