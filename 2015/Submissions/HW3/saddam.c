#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		void *(*start_routine)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long iteration;
int NTHREADS;
//nfot:Number iteration for one thread
long nfot;
unsigned int seed1;
unsigned int seed2;
float PI;
//The number of shots remaining in the quarter circle
int globalSayac = 0;
int tumNoktalar = 0;
int j;
float x, y;

//Her thread yaratılırken bu fonksiyonu paremetre olarak alır
//Kare üzerinde totalde tumNoktalar ve global değişken sayısı kadarda çeyrek daire içinde nokta yaratır(atış yapar)
void* runner(void* arg) {
	pthread_mutex_lock(&mutex);
	for (j = 0; j < nfot; j++) {
		tumNoktalar = tumNoktalar + 1;
		x = rand_r(&seed1) % 1;
		y = rand_r(&seed2) % 1;
		if ((x * x + y * y) <= 1) {
			globalSayac = globalSayac + 1;
		}
	}
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {
	int i;
	seed1 = time(NULL);
	seed2 = time(NULL);
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <Iteration> <NumThread>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	//printf("<iterasyon sayisi> \n");
	iteration = atol(argv[1]);

	//printf("<thread sayisi> \n");
	NTHREADS = atoi(argv[2]);

	nfot = iteration / NTHREADS;

	//Thread dizisi
	pthread_t threads[NTHREADS];
	threads[NTHREADS] = malloc((NTHREADS * sizeof(pthread_t)));

	//NTHREADS adet paralel thread başlatılıyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_create(&threads[i], NULL, runner, NULL);
	}
	// Thread’ler ana süreç tarafından bekleniyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);

	PI = (globalSayac / tumNoktalar) * 4;
	printf("PI = %f\n", PI);

	return 0;
}
