#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

/* global değişkenler tanımlanıyor */
int pointCounter = 0; // çeyrek çemberin içerisinde kalan x,y çiftlerinin sayısını tutar
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long thread_number, iteration_number, iterationPerThread_number;

/* her bir thread'in "iterationPerThread_number" değeri kadar çalıştıracağı ok atma fonksiyonu.
   double x ve y değişkenlerine random sayı atayarak, bu sayıların
   çeyrek birim dairenin içinde olması durumunda counter'ı bir arttırır ve devamında pointCounter'a eklenir */
void* shoot_arrow(void *arg) {
   unsigned int seed;
   double x, y;
   int i, counter = 0;
   for (i = 0; i < iterationPerThread_number; i++) {
       x = rand_r(&seed) / (double)(RAND_MAX);
       y = rand_r(&seed) / (double)(RAND_MAX);
       if ((x*x + y*y) <= 1) {
          counter++;
       }
   }
   pthread_mutex_lock(&mutex);
   pointCounter += counter;
   pthread_mutex_unlock(&mutex);
}

/* main fonksiyonu */
int main(int argc, char *argv[]) {
   /* parametrenin yanlış girilmesi veya --help komutu girilmesi durumunda kullanımı gösterir ve program sonlanır */
   if (argc != 3 || strcmp(argv[1], "--help") == 0) {
       fprintf(stderr, "Usage: %s <iteration_number> <thread_number>\n", argv[0]);
       exit(1);
   }

   /* iterasyon ve thread sayıları parametreden alınıyor */
   iteration_number = atol(argv[1]);
   thread_number = atol(argv[2]);

   /* thread başına düşen iterasyon sayısı hesaplanıyor */
   iterationPerThread_number = iteration_number/thread_number;

   /* threads alanı oluşturuluyor */
   pthread_t *threads = (pthread_t*) malloc(thread_number*sizeof(pthread_t));

   /* thread_number kadar paralel thread oluşturuluyor */
   int i;
   for (i = 0; i < thread_number; i++) {
       pthread_create(&threads[i], NULL, shoot_arrow, NULL);
   }

   /* threadler bekleniyor */
   for (i = 0; i < thread_number; i++) {
       pthread_join(threads[i], NULL);
   }

   /* threads alanı serbest bırakılıyor */
   free(threads);

   /* thread mutex sonlandırılıyor */
   pthread_mutex_destroy(&mutex);

   /* pi sayısı ekrana bastırılıyor */
   printf("Pi Number: %f\n", (4 * (double)pointCounter / (double)iteration_number));   

   return 0;
}