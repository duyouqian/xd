#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

int main(int argc, char **argv)
{
    int32 fileHandler = open("ts_file.log", O_RDWR | O_CREAT, 0777);
    check(fileHandler != -1);
    XDFileHandle fileHandle(fileHandler);
    fileHandle.seekFromEnd();
    const char *msg = "hello ts_file\n";
    fileHandle.write((const uint8*)msg, strlen(msg));
    return 0;
}
