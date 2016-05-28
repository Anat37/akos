
CC = gcc
CFLAGS = -g -c -ansi -w

all: gameserver killer gameclient

gameserver: playerfunc.o hostfunc.o gameserver.o safe_gets.o msg.o
	$(CC) msg.o gameserver.o -lpthread -lm safe_gets.o playerfunc.o hostfunc.o -o gameserver 
	
gameserver.o: gameserver.c header.h serverpart.h daemonbeing.h gameplay.h defs.h
	$(CC) $(CFLAGS) gameserver.c -o gameserver.o 
	
playerfunc.o: playerfunc.c header.h
	$(CC) $(CFLAGS) -o playerfunc.o playerfunc.c 

hostfunc.o: hostfunc.c header.h 
	$(CC) $(CFLAGS) -o hostfunc.o hostfunc.c 

	
header.h: defs.h
  

msg.o:msg.c
	$(CC) $(CFLAGS) -o msg.o msg.c

killer: killer.o 
	$(CC) killer.o -o killer
	
killer.o: killer.c
	$(CC) $(CFLAGS) -o killer.o killer.c 
	
safe_gets.o: ../short_task/safe_gets.c
	$(CC) $(CFLAGS) -o safe_gets.o ../short_task/safe_gets.c 
	
gameclient: safe_gets.o gameclient.o printout.o msg.o defs.h
	$(CC) safe_gets.o msg.o gameclient.o printout.o -o gameclient  
	
gameclient.o: gameclient.c defs.h
	$(CC) $(CFLAGS) gameclient.c -o gameclient.o 

printout.o: printout.c
	$(CC) $(CFLAGS) printout.c -o printout.o

clean:
	rm *.o
