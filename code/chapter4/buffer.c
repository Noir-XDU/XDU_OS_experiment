#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include"buffer.h"
#include<unistd.h>
#include<errno.h>
#include<string.h>



int insert_item(buffer_item item){
  

    if (sem_wait(&empty))   return -1;
    if (pthread_mutex_lock(&mutex))   return -1;
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    if (pthread_mutex_unlock(&mutex))  return -1;
    if(sem_post(&full))  return -1;
    return 0;
}

int remove_item(){
    buffer_item item;

    if (sem_wait(&full))    return -1;
    if (pthread_mutex_lock(&mutex))   return -1;
    item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    if (pthread_mutex_unlock(&mutex))  return -1;
    if(sem_post(&empty))  return -1;
    return item;
    
}

void *producer(void *param){
    buffer_item item;
    size_t name;
    pthread_mutex_lock(&pro_mutex);
    name = ++produce;
    pthread_mutex_unlock(&pro_mutex);

    while ( 1 )
    {
        sleep(rand() % 5);

        item = rand()%100;
        if(insert_item(item))
            printf("\033[31mError: Something wrong !!!\033[m\n%s\n",strerror(errno));
        else
            printf("PRODECER: %zd    PRODUCE: %zd\n",name,item);
        total_produce += item;
    }
    
}

void *consumer(void *param){
    buffer_item item;
    size_t name;
    pthread_mutex_lock(&con_mutex);
    name = ++consume;
    pthread_mutex_unlock(&con_mutex);
    while (1)
    {
        sleep(rand() % 9);
        errno = 0;
        if((item = remove_item()) == -1)
            printf("\033[31mError: Something wrong !!!\033[m\n%s\n",strerror(errno));
        else
            printf("CONSUMER: %zd    CONSUME: %zd\n",name,item);
        total_consume += item;
    }
    
}