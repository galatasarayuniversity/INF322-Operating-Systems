/*
 * main.c
 *
 *  Created on: Apr 27, 2015
 *      Author: cemcelebi
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

long points_per_runner;
long daire_ici;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct point {
	float x;
	float y;
};

long thread_basina_iterasyonu_hesapla(long iterasyon_sayisi, int thread_sayisi) {
	return (iterasyon_sayisi / thread_sayisi);
}

int iceride_mi(struct point p1) {
	if (((p1.x * p1.x) + (p1.y * p1.y)) < 1) {
		return 1;
	}
}
void* runner(void* arg) {
	long daire_ici_thread = 0;
	struct point p1;
	long i = 0;
	unsigned int seed = rand();
	for (i; i < points_per_runner; i++) {
		p1.x = rand_r(&seed) / ((double) RAND_MAX + 1); // +1 cunku 1 uretmesini de istemiyoruz
		p1.y = rand_r(&seed) / ((double) RAND_MAX + 1);
		//printf("p1.x= %f\n",p1.y);
		if (iceride_mi(p1) == 1) {
			daire_ici_thread++;
			//printf("daire_ici_thread= %lu \n",daire_ici_thread);
		}
	}
	pthread_mutex_lock(&mutex);
	daire_ici = daire_ici + daire_ici_thread;
	printf("daire_icine_du$en_nokta_sayisi= %lu \n", daire_ici);
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {
	pthread_t *threads;
	clock_t begin, end;
	begin = clock();
	clock_t tic = clock();
	if (argc != 3) {
		printf("dogru kullanim:<iterasyon sayisi> <thread sayisi> \n");
		exit(1);
	}
	long iterasyon_sayisi = atol(argv[1]);
	printf("\n \niterasyon sayisi: %lu \n", iterasyon_sayisi);
	int thread_sayisi = atoi(argv[2]);
	int NTHREADS = thread_sayisi;
	printf("NTHREADS: %d \n", NTHREADS);
	points_per_runner = thread_basina_iterasyonu_hesapla(iterasyon_sayisi,
			thread_sayisi);
	printf("points per runner: %lu \n", points_per_runner);
	//malloc i$lemi
	threads = malloc(sizeof(pthread_t) * NTHREADS);
	//pthread_t threads[NTHREADS];
	int i;
	// NTHREADS adet paralel thread başlatılıyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_create(&threads[i], NULL, runner, NULL);
	}
	// Thread’ler ana süreç tarafından bekleniyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	free(threads);
	printf("\n|||||||||||||||||||||||Pi: %f\n",
			(4. * (double) daire_ici)
					/ ((double) points_per_runner * NTHREADS));
	clock_t toc = clock();

	printf("------->Elapsed: %f seconds\n \n",
			(double) (toc - tic) / CLOCKS_PER_SEC / NTHREADS); // neden bilmiyorum, terminal'deki time ile ayni zamani alamiyorum
	return 0;
}
