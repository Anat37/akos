#include <stdio.h>


int bitpr(long x, char* ptr);

int main()
{	
	char* str;
	long a = 25; 
	str = malloc(256 * sizeof(char));
	bitpr(25, str);
	printf("%s\n", str);
	return 0;
}
