CC = gcc -g


main: main.o manejoStrings.o
	$(CC) -o main main.o manejoStrings.o

main.o: main.c 
	$(CC) -c main.c 

manejoStrngs.o: manejoStrings.c
	$(CC) -c manejoStrings.c