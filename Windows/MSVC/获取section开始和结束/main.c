#include <stdio.h>

typedef void(*pfunc_t)(void);

#pragma section(".mine$a", read)
__declspec(allocate(".mine$a")) void *InitSegStart;

#pragma section(".mine$m", read)

#pragma section(".mine$z", read)
__declspec(allocate(".mine$z")) void *InitSegEnd;

void InitializeObjects()
{
    const pfunc_t *x = (pfunc_t *)&InitSegStart;
    const pfunc_t *y = (pfunc_t *)&InitSegEnd;

    printf("%p\n", x);
    printf("%p\n", y);
    for (++x; x < y; ++x)
    {
        printf("---> %p\n", x);
        if (*x) (*x)();
    }
}

void function(void)
{
    printf("func\n");
}

__declspec(allocate(".mine$m")) pfunc_t pp = function;
__declspec(allocate(".mine$m")) pfunc_t pp2 = function;

int main()
{
    InitializeObjects();
}
