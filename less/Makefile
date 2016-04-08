CC = gcc
CFLAGS = -g

all: main.o safe_gets.o
	$(CC) $(CFLAGS) main.o safe_gets.o -o lesss
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
	
safe_gets.o: ../short_task/safe_gets.c
	$(CC) $(CFLAGS) -c ../short_task/safe_gets.c -o safe_gets.o

clean:
	rm main.o safe_gets.o lesss
	
	
