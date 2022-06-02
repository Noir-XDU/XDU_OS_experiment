#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>


pthread_t pthread_1,pthread_2,pthread_3;
int *arr,*arr_1;
size_t count, half,n;

typedef struct paramater
{
    int *arr;
    size_t len;
    size_t width;
    int (*cmp) (const void * a, const void * b);

} para;

typedef struct array{
    int *arr;
    size_t numbers;
} arry;

int cmp(const void * a, const void * b)
{   
    return ( *(int *)a - *(int *)b );
}

void *sort(void * p1){
    para *p = (para *)p1;
    qsort(p->arr,p->len,p->width,p->cmp);
    pthread_exit(0);
}

void *sort_last(){
    int ptr=0,i=0,j=half;
    while (i < half && j < count)
    {
        if (arr[i]<arr[j])
        {
            arr_1[ptr] = arr[i];
            i++;
        }else{
            arr_1[ptr] = arr[j];
            j++;
        }
        ptr++;   
    }
    while (i < half)
    {
        arr_1[ptr] = arr[i];
        i++;ptr++;
    }
    while (j < count)
    {
        arr_1[ptr] = arr[j];
        j++;ptr++;
    }
    pthread_exit(0);
}

void check_malloc(void *t){
    if(t == NULL){
        puts("\033[31mError: Fail to malloc !!!\033[m");
        exit(-1);
    }
}

int main(){
    puts("\033[34mPlease input the amount of the numbers you want to input:\033[m");
    scanf("%zd",&count);//首先获取要输入数组的大小
    if(count < 1){
        puts("\033[31mError: The amount of the numbers must be more than 1 !!!\033[m");
        exit(-1);
    }
    half = count/2;
    n = count - half;

    arr = (int *) malloc(sizeof(int)*count);
    arr_1 = (int *) malloc(sizeof(int)*count);
    check_malloc(arr);

    puts("\033[34mPlease input your numbers:\033[m");
    
    size_t i = 0;
    for (size_t i = 0; i < count; i++)
    {
        scanf("%d",arr+i);
    }
    
    
    puts("The following nums will be sorted by thread 1:");
    for (size_t i = 0; i < half; i++)
        printf("%d ", arr[i]);
    puts("");

    puts("The following nums will be sorted by thread 2:");
    for (size_t i = half; i < count; i++)
        printf("%d ", arr[i]);
    puts("");
    para p_1 = {arr,half,sizeof(int),cmp};
    para p_2 = {arr+half,count-half,sizeof(int),cmp};
    pthread_create(&pthread_1, NULL, sort, &p_1);
    pthread_create(& pthread_2, NULL, sort, &p_2);
    pthread_join(pthread_1, NULL);
    pthread_join(pthread_2, NULL);


    arry ar = {arr,count};
    pthread_create(&pthread_3, NULL, sort_last, NULL);
    pthread_join(pthread_3, NULL);
    for (size_t i = 0; i < count; i++)
    {
        printf("%d ",arr_1[i]);
    }
    puts("");
}