#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BITWISE_PRINT(X)                             \
{                                                    \
	int i = 0;                                   \
	for (i; i < sizeof(X) * 8; ++i)              \
	{                                            \
		putchar(((X >> i) & 1) + '0');       \
	}                                            \
}                                                    
/* comm */
int packer(const char* obj, unsigned int** result)
{
	unsigned int buf = 0;
	int cnt = 0;
	int size = 0;
	int i = 0;
	do
	{	
		buf = buf << 3;
		switch(*(obj + i))
		{
			case '\0' :break;
			case 'a' :
				buf += 1; 
				break;
			case 't' :
				buf += 2;
				break;
			case 'g' : 
				buf += 3;
				break;
			case 'c' :
				buf += 4;
				break;
			case 'n' :
				buf += 5;
				break;
		}
		++cnt;
		if (cnt == (sizeof(int) * 8) / 3 )
		{	
			int* arr = (int *) malloc(sizeof(int) * (size + 1));
			if (size > 0)
			{
				memcpy(arr, *result, size);
				free(*result);
			} 
			*result = arr;	
			*(*result + size) = buf;
			buf = 0;
			cnt = 0;
			++size;
		}
		++i;
	} 
	while (*(obj + i) != '\0');
	if (cnt > 0)
	{	
		while (cnt < (sizeof(int) * 8) / 3)
		{
			cnt++;
			buf = buf << 3;
		} 
		int* arr = (int *) malloc(sizeof(int) * (size + 1));
        	if (size > 0) 
		{
			memcpy(arr, *result, size);
        		free(*result);
		}
        	*result = arr;
        	*(*result + size) = buf;
       		buf = 0;
        	cnt = 0;
       		++size;
	}
	i = 0;
	for (i; i < size; ++i)
	{
		int k = *(*result + i);
		BITWISE_PRINT(k);
	}	
	return 0;	
} 

int main()
{
	char string[256];
	fgets(string, 256, stdin);
	unsigned int* buf;
	packer(string, &buf);
	return 0;
}
