#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<math.h>



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned int seed;


long red=0,blue=0;



void* runner(void* arg)
{


	double x,y;
	x=(double)rand_r(&seed) / (double)RAND_MAX ;
	y=(double)rand_r(&seed)/(double)RAND_MAX ;



	if(sqrt((x*x)+(y*y))<=1)
	{
		pthread_mutex_lock(&mutex);
		red++;
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		pthread_mutex_lock(&mutex);
		blue++;
		pthread_mutex_unlock(&mutex);
	}

	return 0;
}

int main(int argc, char *argv[])
{

	if(argc!=3)
	{
		printf("Insufficient argument. Command must be like :(./example 1000000 4)\n ");
		return 0;
	}

	else
	{
		int numThread = atoi(argv[2]);
		long iteration = atol(argv[1])/numThread;

			// Thread dizisi
		//pthread_t threads[numThread];
		pthread_t *threads = malloc(numThread*sizeof(pthread_t));


			int i,j;
			// numThread adet paralel thread başlatılıyor
			for (i = 0; i < numThread; i++)
			{
				for(j=0;j<iteration;j++)
				{
					pthread_create(&threads[i], NULL, runner, NULL);
					//printf("%d\n",i); // Kontrol için!
				}
			}
			// Thread’ler ana süreç tarafından bekleniyor
			for (i = 0; i < numThread; i++)
			{
				pthread_join(threads[i], NULL);
			}

			//pthread_join(*threads, NULL);
			pthread_mutex_destroy(&mutex);
			free(threads);

			double result = (double)(4*(double)red/(double)(red+blue));
			printf("Pi = %lf\n", result);


			return 0;
	}


}

