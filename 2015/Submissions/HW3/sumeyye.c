/*Sumeyye KONAK*/
/*ODEV_3-POSIX_Threads - Monte Carlo - */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long countin = 0;			//noktanin daire icinde oldugu durum sayisini hesaplayacak
long thread_iteration_number = 0;	//thread basina dusen iterasyon sayisini tutacak

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	//bir mutex'i ilklendiriyoruz PTHREAD_MUTEX_INITIALIZER ile

//thread'in calistiracagi isci fonksiyon
void* worker(){
	long i;
	long countin_t = 0;
	unsigned int globalSeed = rand();
		
	//thread uretmesi gereken sayida guvenli olarak random x ve y degerleri uretiyor
	for (i = 0; i < thread_iteration_number; i++){

		double x = rand_r(&globalSeed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
		double y = rand_r(&globalSeed) / ((double)RAND_MAX + 1) * 2.0 - 1.0;

		//birim cemberde calistigimizdan eger 1'den kucukse bu nokta cemberin icinde => counter'i artiriyoruz
		if (((x*x) + (y*y)) < 1){
			countin_t++;
		}
	}
	//kritik kodu mutex'in icine aliyoruz - kilitliyoruz
	pthread_mutex_lock(&mutex);
	countin = countin + countin_t;
	pthread_mutex_unlock(&mutex); //kilidi aciyoruz
}

int main(int argc, const char *argv[]){

	/* (2 komut satiri parametresi + 1 ./dosyaexecutable adi) = 3 parametre alinmadigi surece
	kullanim hakkinda bilgi verip programi exit(1) ile sonlandiriyoruz*/

	if (argc != 3){
		fprintf(stderr, "Kullanim: ./name <iterasyon sayisi> <thread sayisi>\n");
		exit(1);
	}

	//not: argv[0] ./dosyaadi oldugundan onu almayip direk 1 ve 2 yi cekiyoruz
	long total_iteration_number = atol(argv[1]); //iterasyon sayisi bir tamsayinin alabilecegi max degerden fazla olabilir bu yuzden => long
	int thread_number = atoi(argv[2]);

	thread_iteration_number = total_iteration_number/thread_number; //thread basina dusen iteration sayisini hesapliyoruz

	//malloc kullanarak, komut satirinda verilen thread sayisina bagli bir dizi olusturuyoruz, yer aciyoruz
	pthread_t *threads = malloc(thread_number * sizeof(pthread_t));
	//pthread_attr_t turunden bir degisken tanimliyoruz, pthread_create in ikinci parametresi olacak ilerde
	pthread_attr_t attr; 
	//pthread degiskeni olan attr nesnesini ilklendiriyoruz	
	pthread_attr_init(&attr);

	int i;
	//(kullanicinin istedigi)thread sayisi kadar thread yaratip, onlari mallocla yarattigimiz diziye atiyoruz
	for (i = 0; i < thread_number; i++) {
		pthread_create(&threads[i], &attr, worker, (void *) NULL);
	}

	//yarattigimiz her threadi burda bekliyoruz
	for (i = 0; i < thread_number; i++) {
		pthread_join(threads[i], NULL);
	}

	//threadler sonlandiginda ana surec threadleri yok ediyor
	pthread_mutex_destroy(&mutex);

	//mallocla threads icin aldigimiz alani geri veriyoruz
	free(threads);

	//monte carlo'ya gore pi sayisini hesapliyoruz
	double pi = (4*(double)countin) / ((double)thread_number*(double)thread_iteration_number);

	printf("pi: %f\n", pi);
	return 0;
}
