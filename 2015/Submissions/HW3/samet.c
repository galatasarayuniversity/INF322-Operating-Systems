
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 



/* global değişkenler */
long double pi = 0.0;  
pthread_mutex_t Locked;        
long double intervals;
int numThreads;
double numiter;


void *calculPI(void *id)
{	
   unsigned int seed=time(NULL);
   double z,x,y,localSum=0;
   int i,count=0;

	srand(time(NULL));
	 for(i = 0 ; i < numiter; i ++) {
		x = ( double)rand_r(&seed)/RAND_MAX;
	      	y = ( double)rand_r(&seed)/RAND_MAX;
		z = x*x+y*y;
	      	if (z<=1) count++;
	    }

   	 pthread_mutex_lock(&Locked);
  	 pi = (double)(count)/numiter*4;
   	 pthread_mutex_unlock(&Locked); 

    return NULL;
} 

int main(int argc, char **argv)
{
	 
    pthread_t *threads;        
    int i;                   
  
 	if (argc == 3) {
	      intervals = atol(argv[1]); 
	      numThreads = atoi(argv[2]); 
	      threads = malloc(numThreads*sizeof(pthread_t));
	      pthread_mutex_init(&Locked, NULL);
	      numiter = intervals/numThreads;

	      for (i = 0; i < numThreads; i++) {
	       	  pthread_create(&threads[i], NULL, calculPI,NULL);
	      }

	      for (i = 0; i < numThreads; i++) {
		  pthread_join(threads[i], NULL);
	      }
	      
	      printf("Hesaplanan pi sayısı : %32.30Lf \n", pi);
	      
	      printf("Pi sayısının değeri  : 3.141592653589793238462643383279...\n");
    } 
    else {
      printf("ÇALIŞTIRMA ŞEKLİ: ./pi <numIntervals> <numThreads>");    
    }

    return 0;
}

