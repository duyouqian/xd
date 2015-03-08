#include <stdio.h>
#include "common.h"

int main(int argc, char **argv)
{
    printf("now time(s):%ld \n", XDTimer::getCurTimestampByS());
    printf("now time(ms):%ld \n", XDTimer::getCurTimestampByMS());
    printf("now time(us):%ld \n", XDTimer::getCurTimestampByUS());
    printf("now fmtTime:%s \n", XDTimer::getFormatTime().c_str());
    return 0;
}
