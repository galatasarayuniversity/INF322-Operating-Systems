#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define NTHREADS 8

long iterasyon_sayisi, birthread_kaciterasyon;
int thread_sayisi,sayac=0;
float pi;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* runner(void* arg) {
/*The rand() and rand_r() functions return a value between 0 and RAND_MAX
  The seedp argument is a pointer to an unsigned int that
       is used to store state between calls.  If rand_r() is called  with  the
       same  initial value for the integer pointed to by seedp, and that value
       is not modified between calls, then  the  same  pseudo-random  sequence
       will result.
*/
       	long i;
	int icerdemi=0;
 	unsigned int seedp = rand();
/*The C library function int rand(void) returns a pseudo-random number in the range of 0 to RAND_MAX.*/

	for(i=0;i<birthread_kaciterasyon;i++){
		double x = (double)rand_r(&seedp) / RAND_MAX;
        	double y = (double)rand_r(&seedp) / RAND_MAX;

		if(x*x + y*y <= 1){
			icerdemi++;
		}
	}

pthread_mutex_lock(&mutex);
sayac=sayac+icerdemi;				
pthread_mutex_unlock(&mutex);
}

/*sayac global değişken olduğu için butun 
threadler aynanda oynamasın diye kilitledik*/

int main(int argc, char *argv[]){

	iterasyon_sayisi = atol(argv[1]);
	thread_sayisi = atoi(argv[2]);
	birthread_kaciterasyon = iterasyon_sayisi/thread_sayisi;

	// Thread dizisi
	pthread_t *threads = malloc(thread_sayisi*sizeof(pthread_t));
	int i;

	
	// NTHREADS adet paralel thread başlatılıyor
	for (i = 0; i <thread_sayisi ; i++) {
		pthread_create(&threads[i], NULL, runner, NULL);
	}

	// Thread’ler ana süreç tarafından bekleniyor
	for (i = 0; i < thread_sayisi; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	free(threads);
	pi = 4* ( (float)sayac / (float)iterasyon_sayisi);
	printf("PI:%f\n",pi);
printf("sayac:%d\n",sayac);
printf("iterasyon_sayisi:%ld\n",iterasyon_sayisi);
	
	return 0;
}
