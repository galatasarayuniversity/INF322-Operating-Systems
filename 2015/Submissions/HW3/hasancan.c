#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
void* nokta(void*);
void help(char*);

unsigned int seed;
long total = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        help(argv[0]);
    }

    int i, thread_say = atoi(argv[2]);
    int *errCode;
    double pi;

    errCode = (int*)malloc(sizeof(int)*thread_say);

    pthread_t *threads;
    pthread_attr_t attr;

    seed = time(NULL);
    long iterasyon = atol(argv[1])/(long)thread_say;

    threads =  (pthread_t*)malloc(sizeof(pthread_t)*thread_say);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    for (i=0; i < thread_say; i++)
    {
        if (((errCode[i] = pthread_create(&threads[i], &attr, &nokta, (void*)&iterasyon)) != 0))
            printf("ERROR creating thread %d, error=%d\n",i,errCode[i]);
    }
    
    pthread_attr_destroy(&attr);
    
    for (i=0; i < thread_say; i++)
    {

        if (errCode[i]==0)
        {
            errCode[i] = pthread_join(threads[i], NULL);
            if(errCode[i]!=0)
                printf("error joining thread %d, error=%d",i,errCode[i]);
        }
    }
    
    pthread_mutex_destroy(&mutex);

    pi = 4*(double)total/atol(argv[1]);
    printf("pi: %.10f\n",pi);

    free(threads);
    free(errCode);

    return 0;
}
void help(char* arg)
{
    printf("Kullanim: %s <iterasyon sayisi> <thread sayisi>\n" , arg);
    exit(EXIT_FAILURE);
}
void* nokta(void* iter)
{
    double x, y;
    long i,local=0;
    for (i = 0; i < *(long *)iter; i++)
    {
        y = (double)rand_r(&seed)/RAND_MAX;
        x = (double)rand_r(&seed)/RAND_MAX;
        if (((x*x)+(y*y))<= 1.0)
            local += 1;
    }
    pthread_mutex_lock(&mutex);
    total += local;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
