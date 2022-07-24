ALL:
	gcc -c ./src/main.c -g 
	gcc -c ./src/serverInit.c -g 
	gcc -c ./src/readFile.c -g
	gcc -c ./src/handleRequest.c -g
	gcc main.o serverInit.o readFile.o handleRequest.o -o  exec -lpthread
	rm ./main.o
	rm ./serverInit.o
	rm ./readFile.o
	rm ./handleRequest.o
	./exec

clean: 
	rm exec
