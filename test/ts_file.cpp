#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

int main(int argc, char **argv)
{
    XDFile file;
    file.writeOpen("ts_file.log", true, true);
    char readMsg[2560];
    bzero(readMsg, sizeof(readMsg));
    file.read((uint8*)readMsg, sizeof(readMsg));
    printf("[FuleHandle] meaasge:%s\n", readMsg);
    const char *msg = "hello ts_file\n";
    file.write((const uint8*)msg, strlen(msg));
    return 0;
}
