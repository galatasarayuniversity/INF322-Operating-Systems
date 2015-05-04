#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define RAND_MAX 32768


long Points_In_Circle;
long Points_Per_Thread;
float Pi;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* runner(void* arg) {

    int ctr = 0;
    unsigned int random = rand();
    
    for(long i=0 ; i < Points_Per_Thread; i ++ ){
        
        double x = rand_r(&random) / ((double)RAND_MAX + 1) ;
        double y = rand_r(&random) / ((double)RAND_MAX + 1) ;
        
        if( (x*x + y*y) < 1 ){
            ctr ++;
        }
        
    }
    pthread_mutex_lock(&mutex);
    Points_In_Circle += ctr;
    pthread_mutex_unlock(&mutex);
}

int main(int argc, const char *argv[])
{
    if( argc != 3){
        fprintf(stderr, "Usage: ./MonteCarloPi <Number of Points> <Number of Threads> \n");
        exit(1);
    }
    
    
    long Number_of_Points  = atol(argv[1]);
    int  Number_of_Threads = atoi(argv[2]);
    pthread_t *Threads = malloc(Number_of_Threads * sizeof(pthread_t));
   
    Points_Per_Thread = Number_of_Points / Number_of_Threads;
    
    time_t start = time(NULL);
    srand((unsigned)time(NULL));

   
    int i;
    for(i = 0; i < Number_of_Threads; i++){
        pthread_create(&Threads[i], NULL, runner, NULL);
    }
    
    for (i = 0; i < Number_of_Threads; i++) {
        pthread_join(Threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    free(Threads);
    
    Pi = (double) (4. * Points_In_Circle) / (Points_Per_Thread * Number_of_Threads);
    printf("Pi: %f\n", Pi);
    
    return 0;
    
    
}
