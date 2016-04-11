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
volatile short number_switch = 0;
volatile char** data = NULL;
volatile int strcount = 0;
volatile int posx = 0;
volatile int posy = 0;
volatile int height = 0;
volatile int width = 0;
volatile size_t max_len = 0;

void reprint();

int handler(int sig)
{
	if (sig == SIGWINCH)
	{
		get_term_size();
		reprint();
	}
	return 0;
}

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

int options(int argc, char* argv[])
{
	char *opts = "n";
    	char op;
    	int opt; 
    	int ind = 1;
    	while((opt = getopt(argc, argv, opts)) != -1)
    	{  
    		++ind;
        	number_switch = 1;	
        }
        return ind;	
}

int scan(FILE* file)
{
	char* str = NULL;
	size_t siz = strcount;
	size_t len = 0;
	
	if (resize(strcount) == -1)
		return -1;
	
	while (safe_gets(file, &str) == 0)
	{
		data[strcount++] = str;
		len = mystrlen(str);
		if (len > max_len)
			max_len = len;		 
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
    	if (number_switch)
    		--width;
    	height =  ws.ws_row;
	close(fd);
	return 0;
} 

int write_n(size_t from, size_t to, char* str)
{
	char* loc;
	size_t cnt = 0;
	
	for (cnt; cnt < from; ++cnt)
	{
		if (*str == '\n')
		{
			cnt = to;
			printf("\n");
			return -1;
		}
			
		if (*str & (1 << 7))
		{
			if (*str & (1 << 5))
			{
					if (*str & (1 << 4))
					{
						str += 4;
					} else
					{
						str += 3;
					}
			} else 
			{
				str += 2;	
			}
		} else
		{
			++str;
		}
	}
	
	for (cnt; cnt < to; ++cnt)
	{
		if (*str == '\0')
		{
			cnt = to;
			return -1;
		}
			
		
		if (*str & (1 << 7))
		{
			if (*str & (1 << 5))
			{
					if (*str & (1 << 4))
					{
						char buf[5];
						buf[0] = *str;
						++str;
						buf[1] = *str;
						++str;
						buf[2] = *str;
						++str;
						buf[3] = *str;
						++str;
						buf[4] = '\0';
						printf("%s",buf);
					} else
					{
						char buf[4];
						buf[0] = *str;
						++str;
						buf[1] = *str;
						++str;
						buf[2] = *str;
						++str;
						buf[3] = '\0';
						printf("%s",buf);
					}
			} else 
			{	
				char buf[3];
				buf[0] = *str;
				++str;
				buf[1] = *str;
				++str;
				buf[2] = '\0';
				printf("%s",buf);
			}
		} else
		{
			if (*str == 9)
			{
				printf("    ");
				cnt+=3;
			}
			else
				printf("%c", *str);
			++str;
		}
	}
	if (*str != '\0')
		printf("\n");
	return 0;
}

int num_len(int a)
{
	int ans = 0;
	if (a == 0)
		return 1;
	while (a > 0)
	{
		a /= 10;
		++ans;
	}
	return ans;
}

void reprint()
{
	int down, up, i;
	int from, to;
	int wid = 0;
	
	down = (height + posy < strcount) ? height + posy : strcount;
	up = (down - height > 0) ? down - height : 0;
	if (number_switch)
	{
		wid = num_len(down);
		width -= wid + 1;
		to = (width + posx < max_len) ? width + posx : max_len;
		from = (to - width > 0) ? to - width : 0;
		width += wid + 1;
	} else 
	{
		to = (width + posx < max_len) ? width + posx : max_len;
		from = (to - width > 0) ? to - width : 0;
	}
	
	
	for (i = up; i < down; ++i)
	{	
		if (number_switch)
		{
			if (from == 0)
				printf("|");
				else
				printf("<");
		}
		if (number_switch)
		{	
			int k = num_len(i);
			printf("%d", i);
			for (k; k < wid; ++k)
				printf(" ");
			printf(":");	
		}
		write_n(from, to, data[i]);
	} 
	if (strcount < height)
	{
		for (i = strcount; i < height; ++i)
			printf("\n");
	}
}

int mystrlen(const char* str)
{
	int cnt = 0;
	while(*str != '\0')
	{	
		if (*str & (1 << 7))
		{
			if (*str & (1 << 5))
			{
					if (*str & (1 << 4))
					{
						str += 4;
					} else
					{
						str += 3;
					}
			} else 
			{
				str += 2;	
			}
		} else
		{	
			if (*str == 9)
				cnt+=3;
			++str;
		}
		++cnt;
	}
	return cnt;
}

void free_mem()
{
	int i = 0;
	for (i; i < strcount; ++i)
		if (data[i] != NULL)
			free(data[i]);
	free(data);
}
int main(int argc, char* argv[])
{
	int ind;
	char* filename;
	char *locale;
	unsigned int symbol[4];
	FILE* file;
	
	ind = options(argc, argv);
	locale = setlocale(LC_ALL, "" );
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
	to_noncanonical();
	if (scan(file) == -1)
		return -1;
	if (get_term_size() != 0)
		return -1;	
	reprint();	
	symbol[0] = 0;
	symbol[1] = 0;
	symbol[2] = 0;
	while((symbol[2]=getchar())!=EOF)
    	{
    		if (symbol[0] == 27 && symbol[1] == 91 && symbol[2] == 65)
    	        {
    	        	if (posy > 0)
    	        		--posy;
    	        	reprint(); 
    	        }
    	        if (symbol[0] == 27 && symbol[1] == 91 && symbol[2] == 66)
    	        {
    	        	if (posy < strcount - height)
    	        		++posy;
    	        	reprint(); 
    	        }
    	        if (symbol[0] == 27 && symbol[1] == 91 && symbol[2] == 67)
    	        {	
    	        	if (posx < max_len - width)
    	        		++posx;
    	        	reprint(); 
    	        }
    	        if (symbol[0] == 27 && symbol[1] == 91 && symbol[2] == 68)
    	        {
    	        	if (posx > 0)
    	        		--posx;
    	        	reprint(); 
    	        }
    	        if (symbol[2] == 'q')
    	        	break;
        	if (symbol[2] == '\004')
        		break;
    		symbol[0] = symbol[1];
    		symbol[1] = symbol[2];
    	}
    	free_mem();
	to_canonical();
	fclose(file);
	return 0;
}
