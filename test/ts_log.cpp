#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int pipeWrite = -1;
int pipeRead = -1;
int fileFd = -1;

void* witeLog(void *argv)
{
    const char *test = "test111111111111111122\n";
    while (1) {
        write(pipeWrite, test, strlen(test));
        ssize_t len = splice(pipeRead, NULL, fileFd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        printf("[THREAD] pipe:%d errno:%s %d \n", len, strerror(errno), errno);
        len = splice(pipeRead, NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        printf("[THREAD] pipe:%d errno:%s %d \n", len, strerror(errno), errno);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t pid;
    int ret;
    int pipefd[2];
    fileFd = open("log.log", O_WRONLY | O_CREAT , 0666);
    if (fileFd < 0) {
        printf("打开文件失败\n");
        return 0;
    }
    lseek(fileFd, 0, SEEK_END);
    ret = pipe(pipefd);
    if (ret < 0) {
        printf("创建管道失败\n");
        return 0;
    }
    pipeWrite = pipefd[1];
    pipeRead = pipefd[0];
    ret = pthread_create(&pid, NULL, &witeLog, NULL);
    if (ret < 0) {
        printf("[MAIN] 创建线程失败\n");
        return 0;
    }
    pthread_join(pid, NULL);
    return 0;
}
