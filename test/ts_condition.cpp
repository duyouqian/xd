#include <stdio.h>
#include "common.h"
#include <pthread.h>
#include <vector>
#include <unistd.h>

std::vector<uint32> vec;

XDMutex mutex_;
XDCondition cond_(&mutex_);

void *thread1(void *argv)
{
    printf("[THREAD1] exec \n");
    uint32 count = 0;
    while (1) {
        // 生产数据
        {
            XDGuardMutex lock(&mutex_);
            vec.push_back(count++);
            printf("[THREAD1] add data len:%d \n", vec.size());
            cond_.signal();
        }
        sleep(1);
    }
    return NULL;
}

void *thread2(void *argv)
{
    printf("[THREAD2] exec \n");
    while (1) {
        std::vector<uint32> temp;
        {
            XDGuardMutex lock(&mutex_);
            while (vec.empty()) {
                printf("[THREAD2] vec is empty \n");
                cond_.wait();
            }
            vec.swap(temp);
        }
        std::vector<uint32>::const_iterator it;
        for (it = temp.begin(); it != temp.end(); ++it) {
            printf("[THREAD2] %d \n", *it);
        }
    }
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
