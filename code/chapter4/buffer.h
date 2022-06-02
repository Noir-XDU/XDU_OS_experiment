#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include"buffer.h"
#include<unistd.h>


typedef size_t buffer_item;
#define BUFFER_SIZE 5
buffer_item buffer[BUFFER_SIZE];
size_t sleep_time, pro_size, con_size;
sem_t empty,full;
pthread_t * producer_thread, * consumer_thread;
size_t in=0,out=0,count=0;
size_t produce=0,consume=0;
pthread_mutex_t mutex, pro_mutex, con_mutex;
size_t total_produce=0,total_consume=0;
