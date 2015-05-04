/*
 * Odev3_HalilYavuz_Cevik_10401569.c
 *
 *  Created on: Apr 27, 2015
 *      Author: yc
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

long NTHREADS = 8;
double pi = 0;
long t_atis;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* runner(void* arg) {

	long a;
	int pi_t = 0;
	unsigned int random = rand();
	unsigned int* random_ptr = &random;

	for (a = 0; a < t_atis; a++) {

		double k = rand_r(random_ptr) / ((double) RAND_MAX + 1) * 2.0 - 1.0;
		double l = rand_r(random_ptr) / ((double) RAND_MAX + 1) * 2.0 - 1.0;

		if (k * k + l * l < 1) {
			pi_t = pi_t + 1;
		}

	}

	pthread_mutex_lock(&mutex);
	pi = pi + pi_t;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {
// Thread dizisi
	pthread_t *threads = malloc(NTHREADS * sizeof(pthread_t));

	int i;

	long totalpoints = atol(argv[1]);
	int NTHREADS = atoi(argv[2]);
	t_atis = totalpoints / NTHREADS;

	// NTHREADS adet paralel thread başlatılıyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_create(&threads[i], NULL, runner, NULL);
	}
// Thread’ler ana süreç tarafından bekleniyor
	for (i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);


	printf(" Pi Sayisi: %f\n", (4. * pi) / ((double) t_atis * NTHREADS));

	return 0;
}
