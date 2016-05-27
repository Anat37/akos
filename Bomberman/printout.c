#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

static struct termios stored_settings;

extern char** map;
extern int game_status;
extern int minecnt;
extern float hp;

void to_noncanonical()
{
	struct termios new_settings;

	tcgetattr(0,&stored_settings);

	new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 3; 
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void to_canonical()
{
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}


void reprint()
{
	int i;
	for (i = 0; i < 20; ++i)
		printf("%s\n", map[i]);
	printf("Healthpoints:%d\n", hp);
	printf("Mines:%d\n", minecnt);
}

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4 
void user_input(int type)
{
	unsigned int symbol;
	if (type == 1)
	{
		reprint();	
		symbol = getchar();
		
		switch (symbol){
			case 'w': move(UP); break;
			case 'a': move(LEFT); break;
			case 's': move(DOWN); break;
			case 'd': move(RIGHT); break;
			case 'm': mine(); break;
			case 'u': use_obj(); break;			
			case 'q': handler(SIGINT);break;
			case 'f': fireuse(); break;
		}
		if (symbol == '\004')
			handler(SIGINT);
	} else 
	{
		reprint();
		symbol = getchar();
		if (symbol == 's')
			game_status = 1;
		if (symbol == 'q')
			handler(SIGINT);
		if (symbol == '\004')
			handler(SIGINT);	
	}
}
