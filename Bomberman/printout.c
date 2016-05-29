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

extern volatile char** map;
extern volatile int game_status;
extern volatile int minecnt;
extern volatile float hp;
volatile int height = 0;
volatile int width = 0;



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

int get_term_size()
{
	int rc, fd;
	struct winsize ws;

    	fd = open( "/dev/tty", O_RDONLY );
    	if( fd < 0 )
    	{
        	perror( "open()" );
        	return 1;
    	}

    	rc = ioctl( fd, TIOCGWINSZ, &ws );
    	if( rc < 0 )
    	{
        	perror( "ioctl()" );
        	return 1;
    	}

    	width = ws.ws_col;
    	height =  ws.ws_row;
	close(fd);
	return 0;
} 

void reprint()
{
	int i;
  get_term_size();
	for (i = 0; i < 20; ++i)
		printf("%s\n", map[i]);
	printf("Healthpoints:%f\n", hp);
	printf("Mines:%d\n", minecnt);
  for (i = 0; i < height - 22; ++i)
    printf("\n");
  fflush(stdout);
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
		symbol = getchar();
		if (symbol == -1)
    printf("bad in!\n");
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
		symbol = getchar();
		if (symbol == 's')
			++game_status;
		if (symbol == 'q')
			handler(SIGINT);
		if (symbol == '\004')
			handler(SIGINT);	
	}
}
