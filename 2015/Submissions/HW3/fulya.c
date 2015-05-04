#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
long birim_iterasyon;    //thread basina düsen iterasyon sayısı
double Pi;
long circleCount = 0; /* Global Circle Count */

pthread_t *threads; // Thread Identifier Decleration 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void* monte_carlo(void* arg){
	long i;
	long incircle_thread = 0;
	unsigned int rand_state = rand();
	for (i = 0; i < birim_iterasyon; i++){
		double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
		double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
		if ((x*x + y*y)<1){
			incircle_thread++;
		}
	}
	Pi = (4. * (double)incircle_thread) /((double)birim_iterasyon * 1);
	pthread_mutex_lock(&mutex);
	circleCount += incircle_thread;
	pthread_mutex_unlock(&mutex);


}



int main(int argc, char *argv[])
{     int i ;
long iterasyon_sayisi=atol(argv[1]);
int thread_sayisi=atoi(argv[2]);
birim_iterasyon=(iterasyon_sayisi/thread_sayisi);
//printf("%lu",birim_iterasyon);
if (argc != 3 || strcmp(argv[1], "--help") == 0) {
	fprintf(stderr, "Usage: %s <iterasyon sayisi> <thread sayisi>\n", argv[0]);   
}
threads = malloc(sizeof(pthread_t)*thread_sayisi);
for (i = 0; i < thread_sayisi; i++) {
	pthread_create(&threads[i], NULL, monte_carlo, NULL);
}
for (i = 0; i < thread_sayisi; i++) {
	pthread_join(threads[i], NULL);
}

printf("Pi = %lf\n", Pi);
}


