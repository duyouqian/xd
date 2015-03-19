#include "common.h"

int main(int argc, char **argv)
{
    XDLOG_open("log", 0);
    XDLOG_close();
    return 0;
}
