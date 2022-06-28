ALL:
	gcc -c main.c -g 
	gcc -c serverInit.c -g 
	gcc -c readFile.c -g
	gcc main.o serverInit.o readFile.o -o  exec -lpthread
	rm main.o
	rm serverInit.o
	rm readFile.o
	./exec

clean: 
	rm exec
