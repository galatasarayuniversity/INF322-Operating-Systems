/*
 * odev3.c
 *
 *  Created on: Apr 29, 2015
 *      Author: seyit
 */



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


long is_in = 0;
long ipert;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* archer(void* arg)
{

	int i;
	int j = 0;
	double x,y;
	unsigned int seed = time(NULL);

	for(i = 0; i < ipert; i++)
	{

		x = ((double)rand_r(&seed)/(RAND_MAX));

		y = ((double)rand_r(&seed)/ (RAND_MAX));

		if(x*x + y*y <= (double)1)
			j++;
	}

	pthread_mutex_lock(&mutex);
	is_in = is_in + j;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
	double Pi;
	long val_i,val_t;					//val_i: iterasyon sayısı, val_t: thread sayısı
	val_i = atol(argv[1]);
	val_t = atol(argv[2]);

	ipert = val_i/val_t;				//ipert: thread başına düşen iterasyon sayısı

	// Thread dizisi
	pthread_t threads[val_t - 1];
	int i;

	// NTHREADS adet paralel thread başlatılıyor
	for (i = 0; i < val_t; i++)
	{
		pthread_create(&threads[i], NULL, archer, NULL);
	}

	// Thread’ler ana süreç tarafından bekleniyor
	for (i = 0; i < val_t; i++)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);

	Pi =  4*((double)is_in/val_i);
	printf("\nPi:  %lf\n",Pi);

	return 0;
}
