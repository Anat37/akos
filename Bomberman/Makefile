
CC = gcc
CFLAGS = -g -c -o

all: gameserver killer

gameserver: gameserver.o safe_gets.o
	$(CC) gameserver.o safe_gets.o -o gameserver
	
gameserver.o: gameserver.c
	$(CC) $(CFLAGS) gameserver.o gameserver.c
	
killer: killer.o 
	$(CC) killer.o -o killer
	
killer.o: killer.c
	$(CC) $(CFLAGS) killer.o killer.c 
	
safe_gets.o: ../short_task/safe_gets.c
	$(CC) $(CFLAGS) safe_gets.o ../short_task/safe_gets.c 

clean:
	rm gameserver.o
