CC = gcc -g -pthread

main: main.o buscaPalindromos.o manejoStrings.o 
	$(CC) -o main main.o buscaPalindromos.o manejoStrings.o 

main.o: main.c buscaPalindromos.h 
	$(CC) -c main.c 

buscaPalindromos.o: buscaPalindromos.c buscaPalindromos.h manejoStrings.h
	$(CC) -c buscaPalindromos.c 

manejoStrngs.o: manejoStrings.c manejoStrings.h
	$(CC) -c manejoStrings.c
