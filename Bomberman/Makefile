
CC = gcc
CFLAGS = -g -c

all: gameserver killer gameclient header.h

gameserver: gameserver.o safe_gets.o  header.h msg.h serverpart.h daemonbeing.h
	$(CC) gameserver.o -lpthread safe_gets.o -o gameserver 
	
gameserver.o: gameserver.c header.h serverpart.h daemonbeing.h msg.h
	$(CC) $(CFLAGS) gameserver.c -lpthread -o gameserver.o 
	
killer: killer.o 
	$(CC) killer.o -o killer
	
killer.o: killer.c
	$(CC) $(CFLAGS) killer.o killer.c 
	
safe_gets.o: ../short_task/safe_gets.c
	$(CC) $(CFLAGS) safe_gets.o ../short_task/safe_gets.c 
	
gameclient: gameclient.o header.h msg.h
	$(CC) gameclient.o -o gameclient 
	
gameclient.o: gameclient.c header.h msg.h
	$(CC) $(CFLAGS) gameclient.c -o gameclient.o 

clean:
	rm gameserver.o gameclient.o
