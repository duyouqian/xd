#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count = 0;
pthread_mutex_t mutex;

void* thread1(void *argv)
{
    while (1) {
        printf("[THREAD1] curCount=%d  ", count);
        pthread_mutex_lock(&mutex);
        ++count;
        pthread_mutex_unlock(&mutex);
        printf("[THREAD1] rsCount=%d  \n", count);
        usleep(1000);
    }
    return NULL;
}

void* thread2(void *argv)
{
    while (1) {
        printf("[THREAD2] curCount=%d  ", count);
        pthread_mutex_lock(&mutex);
        ++count;
        pthread_mutex_unlock(&mutex);
        printf("[THREAD2] rsCount=%d  \n", count);
        usleep(1000);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t pid1, pid2;
    int ret;
    ret = pthread_mutex_init(&mutex, NULL);
    if (ret < 0) {
        printf("[MAIN] 创建互斥量失败\n");
        return 0;
    }
    ret = pthread_create(&pid1, NULL, &thread1, NULL);
    if (ret < 0) {
        printf("[MAIN] 创建线程失败\n");
        return 0;
    }
    ret = pthread_create(&pid2, NULL, &thread2, NULL);
    if (ret < 0) {
        printf("[MAIN] 创建线程失败\n");
        return 0;
    }
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}
