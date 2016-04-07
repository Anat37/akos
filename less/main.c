#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

static struct termios stored_settings;
short number_switch = 0;

void to_noncanonical()
{
	struct termios new_settings;

	tcgetattr(0,&stored_settings);

	new_settings = stored_settings;

	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void to_canonical()
{
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

int options(int argc, char* argv[])
{
	char *opts = "n";
    	char op;
    	int opt; 
    	int ind = 1;
    	while((opt = getopt(argc, argv, opts)) != -1)
    	{ 
    		if (optind == ind) 
    			++ind;
        	number_switch = 1;	
        }
        return ind;	
}

int main(int argc, char* argv[])
{
	int ind;
	char* filename;
	FILE* file;
	ind = options(argc, argv);
	if (argv[ind] == NULL)
	{
		perror("No filename");
		return 1;
	}
	filename = argv[ind];
	file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Open file error");
		return 1;
	}
	to_noncanonical();
	usleep(100000);
	
	getc(stdin);
	to_canonical();
	return 0;
}
