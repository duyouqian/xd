#include <stdio.h>
#include "atomics.h"
#include "types.h"

volatile int32 value;

int main(int argc, char **argv)
{
    printf("value= %d ++value = %d vlaue = %d\n", value,  XDAtomics::interlockedPreIncrement(&value), value);
    printf("value= %d value++ = %d value = %d \n", value,  XDAtomics::interlockedIncrement(&value), value);
    printf("value= %d --value = %d vlaue = %d\n", value,  XDAtomics::interlockedPreDecrement(&value), value);
    printf("value= %d value-- = %d value = %d \n", value,  XDAtomics::interlockedDecrement(&value), value);
    printf("value= %d value + 3 = %d vlaue = %d\n", value,  XDAtomics::interlockedAdd(&value, 3), value);
    printf("value= %d value = 6 %d vlaue = %d\n", value,  XDAtomics::interlockedExchange(&value, 6), value);
    printf("value= %d if value == 5 than value = 10 %d vlaue = %d\n", value,  XDAtomics::interlockedCompareExchange(&value, 10, 5), value);
    return 0;
}
