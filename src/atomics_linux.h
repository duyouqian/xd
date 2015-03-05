#ifndef XD_ATOMICSIMP_LINUX_H
#define XD_ATOMICSIMP_LINUX_H

#include "types.h"

struct XDAtomicsLinuxImp
{
    // value++
    static FORCEINLINE int32 interlockedIncrement(volatile int32* value)
    {
        return __sync_fetch_and_add(value, 1) + 1;
    }

    // value++
    static FORCEINLINE int64 interlockedIncrement(volatile int64* value)
    {
        return __sync_fetch_and_add(value, 1) + 1;
    }

    // ++value
    static FORCEINLINE int32 interlockedPreIncrement(volatile int32* value)
    {
        return __sync_add_and_fetch(value, 1);
    }

    // ++value
    static FORCEINLINE int64 interlockedPreIncrement(volatile int64* value)
    {
        return __sync_add_and_fetch(value, 1);
    }

    // value--
    static FORCEINLINE int32 interlockedDecrement(volatile int32* value)
    {
        return __sync_fetch_and_sub(value, 1) - 1;
    }

    // value--
    static FORCEINLINE int64 interlockedDecrement(volatile int64* value)
    {
        return __sync_fetch_and_sub(value, 1) - 1;
    }

    // --value
    static FORCEINLINE int32 interlockedPreDecrement(volatile int32* value)
    {
        return __sync_sub_and_fetch(value, 1);
    }

    // --value
    static FORCEINLINE int64 interlockedPreDecrement(volatile int64* value)
    {
        return __sync_sub_and_fetch(value, 1);
    }

    // value + amount
    // return old value
    static FORCEINLINE int32 interlockedAdd(volatile int32* value, int32 amount)
    {
        return __sync_fetch_and_add(value, amount);
    }

    // value + amount
    // return old value
    static FORCEINLINE int64 interlockedAdd(volatile int64* value, int64 amount)
    {
        return __sync_fetch_and_add(value, amount);
    }

    // value = amount
    static FORCEINLINE int32 interlockedExchange(volatile int32* value, int32 amount)
    {
        return __sync_lock_test_and_set(value, amount);
    }

    // value = amount
    static FORCEINLINE int64 interlockedExchange(volatile int64* value, int64 amount)
    {
        return __sync_lock_test_and_set(value, amount);
    }

    // value = amount
    static FORCEINLINE void* interlockedExchangePtr(void** value, void* amount)
    {
        return __sync_lock_test_and_set(value, amount);
    }

    // value = amount
    static FORCEINLINE int32 interlockedCompareExchange(volatile int32* value, int32 amount, int32 comperand)
    {
        return __sync_val_compare_and_swap(value, comperand, amount);
    }

#ifdef PLATFORM_64BITS
    // value = amount
    static FORCEINLINE int64 interlockedCompareExchange(volatile int64* value, int64 amount, int64 comperand)
    {
        return __sync_val_compare_and_swap(value, comperand, amount);
    }
#endif

    // value = amount
    static FORCEINLINE void* interlockedCompareExchangePtr(void** value, void* amount, void* comperand)
    {
        return __sync_val_compare_and_swap(value, comperand, amount);
    }
};

#endif // end xd_atomicsimp_linux_h
