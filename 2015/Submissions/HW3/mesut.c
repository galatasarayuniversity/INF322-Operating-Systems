#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#define RANd_MAX 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long niter;
int nthread;
double pi;
int count;


void* MonteCarlo(void* arg){

   //double x,y;
   int i,t=0; 
   double z;


   /* initialize random numbers */
	 
   t=niter/nthread;
   count=0;
   for ( i=0; i<t; i++) {
	int random_state = rand();
	double x = rand_r(&random_state) / ((double)RAND_MAX + 1);
	double y = rand_r(&random_state) / ((double)RAND_MAX + 1);
      //x = (double)rand()/RAND_MAX;
      //y = (double)rand()/RAND_MAX;
      z = x*x+y*y;
      //printf("%f\n",x);
      if (z<1) {
	pthread_mutex_lock(&mutex);
	count++;
   	pthread_mutex_unlock(&mutex);

}
      }

}

main(int argc, const char* argv[]) 
{
        niter=atol(argv[1]); 
        nthread=atoi(argv[2]);
	pthread_t *thread = malloc(sizeof(pthread_t)*nthread);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int i;

	for (i = 0; i < nthread; i++) {
		pthread_create(&thread[i], NULL, MonteCarlo, NULL);
	}

	for (i = 0; i < nthread; i++) {
		pthread_join(thread[i], NULL);
	}
         
          free(thread);
 	pthread_mutex_destroy(&mutex);
        pi=(double)count/niter*4; 
        printf(" estimate of pi is %g \n",pi); 

   
}