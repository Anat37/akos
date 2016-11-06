#include <pthread.h>
#include<stdio.h>

size_t thread_cnt;
size_t matrix_size;
int finished  = 0;


pthread_t* arr;

struct sthread {
    	int id;
    	int count; 
    	pthread_cond_t cond;
    	pthread_mutex_t mutex;
};
struct sthread* arrs;

int** matrix = NULL;
pthread_mutex_t* mutex = NULL;
pthread_mutex_t glob;
pthread_cond_t global;

void* player(void* ar)
{
	int i = 0;
	int x, y, k = 0;

	struct sthread* arg = ar;
	for (k = 0; k < 10; ++k)
	{
		for (i = 0; i < 2; ++i)
		{
			pthread_mutex_lock(&glob);
			x = rand() % matrix_size;
			y = rand() % matrix_size;
			pthread_mutex_unlock(&glob);
			pthread_mutex_lock(mutex + x);
			if ( *(*(matrix + x) + y) == 0)
				{
					++arg->count;
					*(*(matrix + x) + y) = arg->id;
				} else 
				{
					pthread_mutex_lock(&glob);
					sleep(rand() % 6);
					pthread_mutex_unlock(&glob);
				}
			pthread_mutex_unlock(mutex + x);
		}
		
		printf("Thread #%d had %d points\n", arg->id, arg->count);
		arg->count = 0;
		pthread_mutex_lock(&glob);
		++finished;
		if (finished == thread_cnt)
			{	
				int j;
				pthread_mutex_unlock(&glob);
				finished = 0;
				printf("End round\n");
				for (j = 0; j < thread_cnt; ++j)
					pthread_cond_signal(&(arrs[j].cond));
			}
		else {
			pthread_mutex_unlock(&glob);
			pthread_mutex_lock(&(arg->mutex));
			pthread_cond_wait(&(arg->cond), &(arg->mutex));
			pthread_mutex_unlock(&(arg->mutex));
			
		}
		
		
	}
}


int main(int argc, char* argv[])
{
	int i = 0;
	if (argc < 3)
	{
		printf("Few args");
		return 0;
	}
	thread_cnt = atoi(argv[2]);
	matrix_size = atoi(argv[1]);
	arr = malloc(thread_cnt * sizeof(pthread_t));
	arrs = malloc(thread_cnt * sizeof(struct sthread));
	mutex = malloc(matrix_size* sizeof(pthread_mutex_t));
	matrix = malloc(matrix_size * sizeof(int*));
	for (i = 0; i < matrix_size; ++i)
	{
		int j = 0;
		*(matrix + i) = malloc(matrix_size * sizeof(int));
		for (j = 0; j < matrix_size; ++j)
			*(*(matrix + i) + j) = 0;
	}	
	for (i = 0; i < matrix_size; ++i)
		pthread_mutex_init(mutex + i, NULL);
	pthread_mutex_init(&glob, NULL);
	pthread_cond_init(&global, NULL);
	for (i = 0; i < thread_cnt; ++i)
	{	
		(arrs + i)->id = i;
		(arrs + i)->count = 0;
		pthread_cond_init(&(arrs[i].cond), NULL);
    		pthread_mutex_init(&(arrs[i].mutex), NULL);
	}
	for (i = 0; i < thread_cnt; ++i)
		pthread_create(&arr[i], NULL, player, arrs + i); 
		
	for (i = 0; i < thread_cnt; ++i)
		pthread_join(arr[i], NULL);
	for (i = 0; i < matrix_size; ++i)
	{	
		int j;
		for (j = 0; j < matrix_size; ++j)
			printf("%d ", *(*(matrix + i) + j));
		printf("\n"); 
		free(*(matrix + i));
	}
	free(matrix);
	free(arrs);
	free(arr);
	free(mutex);
	return 0;
}
