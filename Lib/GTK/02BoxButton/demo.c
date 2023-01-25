#include <stdio.h>

typedef void (*cb_t)(void);
typedef void (*cb_act_t)(void *);

void set_cb(cb_t cb, void *args)
{
    cb_act_t f = (cb_act_t)cb;
    f(args);
}

#define CALLBACK(x) (cb_t)(x)

void myf(void *arg)
{
    if(arg)
        printf("%s\n", (char*)arg);
}

void fargs(void *arg, int a, int b)
{
    if(arg)
        printf("%s\n", (char*)arg);
}

void fnoarg(void)
{
    printf("No args func\n");
}

int main(int argc, char const *argv[])
{
    set_cb(CALLBACK(myf), "Hello");
    set_cb(CALLBACK(fargs), "Hello2");
    set_cb(CALLBACK(fnoarg), NULL);
    
    return 0;
}

