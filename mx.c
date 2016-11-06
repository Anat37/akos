struct work
{
	int start, finish;
	int state;
	double max;
} works[5];

int main()
{
	int i = 0;
	pthread_t mas[5];
	for (i = 0; i < 5; ++i)
	{
		
		
		
		pthread_create(mas+i, NULL, body, (void*) works+i);
	}
	
}
