#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>

static struct termios stored_settings;
short number_switch = 0;
short double_size = 0;
char** data = NULL;
int strcount = 0;
int posx = 0;
int posy = 0;
int height = 0;
int width = 0;

int handler(int sig)
{
	if (sig == SIGWINCH)
	{
		get_term_size();
		
	}
	return 0;
}

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

int get_loc(char* filename)
{
	pid_t pid;
	int pipe1[2];
	char str[100];
	int status;
	
	if (pipe(pipe1))
	{
		perror("Pipe create");
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		return -1;
	}
	if (pid == 0)
	{
		close(pipe1[0]);
		dup2(pipe1[1], 1);
		execlp("file", "file", "-bi", filename, NULL);
		return -1;
	}
	read(pipe1[0], &str, 100);
	close(pipe1[0]);
	close(pipe1[1]);
	waitpid(pid, &status, 0);
	return 0;
}

int scan(FILE* file)
{
	char* str = NULL;
	size_t siz = strcount;
	
	if (resize(strcount) == -1)
		return -1;
	
	while (safe_gets(file, &str) == 0)
	{
		data[strcount++] = str;
		str = NULL;
		if (strcount > siz * 2)
		{
			if (resize(strcount) == -1)
				return -1;
			siz = strcount;
		}
	}
	return 0;
}

int resize(size_t siz)
{	
	char** ptr;
	siz = (siz * 2) + 10;
	ptr =(void *) realloc((void *)data, siz * sizeof(char*));
	if (ptr == NULL)
	{
		perror("Unable to load file");
		return -1;
	}
	data = ptr;
	return 0;
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
    	printf("%d %d\n", height, width);
	close(fd);
	return 0;
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
	
	signal(SIGWINCH, &handler);
	setvbuf(stdout, NULL, _IONBF, 0);
	to_noncanonical();
	if (scan(file) == -1)
		return -1;
	if (get_term_size() != 0)
		return -1;	
	printf("Тестим русский\n");
	getc(stdin);
	to_canonical();
	return 0;
}
