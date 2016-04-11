#include<stdio.h>

#define BITWISE_PRINT(X){							\
	int i = 0;								\
	for (i; i < sizeof(X) * 8; ++i)						\
	{									\
		if (i % 8 == 0 && i != 0)					\
		{								\
			printf("|");						\
		} else								\
			if (i % 4 == 0 && i != 0)				\
				printf(" "); 					\
		if (X & (1 << (sizeof(X) * 8 - i - 1)))				\
			printf("1");						\
			else							\
			printf("0");						\
	}									\
	printf("\n");								\
										\
	for (i = 0; i < sizeof(X) * 2; ++i) 					\
	{					 				\
		printf("   %X ",(X & (15 << (sizeof(X) * 8 - i * 4 - 4))));	\
										\
	}									\
	printf("\n");								\
										\
}										\

int main()
{
	int a = 5;
	BITWISE_PRINT(a);
	return 0;
}
