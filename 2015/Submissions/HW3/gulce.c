#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
long cember = 0;
long nokta_sayisi; //thread başına atılıcak nokta sayısı
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *runner(void *arg) {
	long runner_ici = 0;
	unsigned int rand_state = rand();
	long i;
	for (i = 0; i < nokta_sayisi; i++) {
		double z;
		double x = rand_r(&rand_state) / ((double) RAND_MAX + 1);
		double y = rand_r(&rand_state) / ((double) RAND_MAX + 1);
		z = x * x + y * y;
		if (z <= 1) {
			runner_ici++;
		}
	}
	pthread_mutex_lock(&mutex);
	cember = runner_ici + cember;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, const char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "  <iterasyon sayisi> <thread sayisi>\n");
		exit(1);
	}
	long toplam_nokta = atol(argv[1]);
	int thread_say = atoi(argv[2]);
	nokta_sayisi = (toplam_nokta / thread_say);

	time_t start = time(NULL);
	srand((unsigned) time(NULL));
	pthread_t *threads = malloc(thread_say * sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int i;
	for (i = 0; i < thread_say; i++) {
		pthread_create(&threads[i], &attr, runner, (void *) NULL);
	}
	for (i = 0; i < thread_say; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&mutex);
	free(threads);
	printf("Pi: %f\n",
			(4. * (double) cember) / ((double) nokta_sayisi * thread_say));

	return 0;
}
