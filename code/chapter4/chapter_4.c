#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include"buffer.c"
#include<unistd.h>



int main(int argc,char *argv[]){
    if(argc != 4){
        puts("\033[31mUsage: ./chapter_4 sleep_time producer_size consumer_size.\033[m");
        exit(0);
    }

    sleep_time = atoi(argv[1]);
    pro_size = atoi(argv[2]);
    con_size = atoi(argv[3]);

    if (sleep_time <= 0 || pro_size <= 0 || con_size <= 0)
    {
        puts("\033[31mError: the arguments must be more than 0.\033[m");
        return 0;
    }

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&con_mutex, NULL);
    pthread_mutex_init(&pro_mutex, NULL);
    consumer_thread = (pthread_t*) malloc(sizeof(pthread_t) * con_size);
    producer_thread = (pthread_t*) malloc(sizeof(pthread_t) * pro_size);


    srand(time(NULL));

    for (int i = 0; i < pro_size;i++)
        pthread_create(producer_thread + i, NULL, producer, NULL);


    for (int i = 0; i < con_size;i++)
        pthread_create(consumer_thread + i, NULL, consumer, NULL);

  
    sleep(sleep_time);
    pthread_join(*producer_thread,NULL);
    pthread_mutex_lock(&mutex);
    printf("totally produce: %zd, totally consume: %zd\n", total_produce, total_consume);
 
    exit(0);

}