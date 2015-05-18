#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


long int n_threads=0,n_iter = 0,sum=0;

long int i;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void* runner(void* arg) {
	
	double x,y;
	unsigned int value;
	long int ctr = 0,k=0;
	
	for(k=0;k<n_iter/n_threads;k++){
	x = rand_r(&value)/(double)RAND_MAX;
	y = rand_r(&value)/(double)RAND_MAX;
	if(x*x+y*y<=1) ctr ++;
	}
	
	pthread_mutex_lock(&mutex);
	sum = sum + ctr;
	pthread_mutex_unlock(&mutex);
}


int main(int argc, char *argv[])
{
n_iter = atol(argv[1]);
n_threads = atol(argv[2]);

pthread_t *threads;

threads = malloc(n_threads*sizeof(pthread_t));
// NTHREADS adet paralel thread başlatılıyor

for (i = 0; i < n_threads; i++) {
pthread_create(&threads[i], NULL, runner, NULL);
}
// Thread’ler ana süreç tarafından bekleniyor
for (i = 0; i < n_threads; i++) {
pthread_join(threads[i], NULL);
}
pthread_mutex_destroy(&mutex);
free(threads);
printf("pi = %g\n", (double)4*sum/n_iter); // Daima 8 basacak.
return 0;
}
