#include "mutex.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "types.h"

XDMutex mutex_;

int32 count;

void *thread1(void *argv)
{
    printf("[THREAD1] exec \n");
    while (1) {
        printf("[THREAD1] %d \n", count);
        {
            XDGuardMutex lock(&mutex_);
            ++count;
        }
            sleep(1);
        printf("[THREAD1] %d \n", count);
    }
    return NULL;
}

void *thread2(void *argv)
{
    printf("[THREAD2] exec \n");
    while (1) {
        printf("[THREAD2] %d \n", count);
        {
            XDGuardMutex lock(&mutex_);
            ++count;
        }
        sleep(1);
        printf("[THREAD2] %d \n", count);
    }
    return NULL;
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t pid1, pid2;
    if (pthread_create(&pid1, NULL, &thread1, NULL) < 0) {
        printf("[MAIN} 创建线程失败 \n");
        return 0;
    }
    if (pthread_create(&pid2, NULL, &thread2, NULL) < 0) {
        printf("[MAIN} 创建线程失败 \n");
        return 0;
    }
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}
