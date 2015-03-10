#include <stdio.h>
#include "common.h"

class Thread1Run : public XDIRunnable
{
public:
    int32 run()
    {
        printf("[Thread1Run] run \n");
    }
};

int main(int argc, char **argv)
{
    Thread1Run tr1;
    tr1.run();
    return 0;
}
