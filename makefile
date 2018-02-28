CC = gcc -g -pthread

main: main.o manejoStrings.o #palindromo.o
	$(CC) -o main main.o manejoStrings.o #palindromo.o

main.o: main.c 
	$(CC) -c main.c 

manejoStrngs.o: manejoStrings.c
	$(CC) -c manejoStrings.c

# palindromo.o: palindromo.c
# 	$(CC) -c palindromo.c