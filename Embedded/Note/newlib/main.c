#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    printf("Hello World\n");
    
    return 0;
}

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    
}

int _close_r(struct _reent *ptr, int fd)
{
    return 0;
}


int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
    return 0;
}

int _isatty_r(struct _reent *ptr, int fd)
{
    return 0;
}

_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
    return 0;
}

_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t nbytes)
{
    return 0;
}

_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t nbytes)
{
    return 0;
}

/* for exit() and abort() */
__attribute__ ((noreturn)) void _exit (int status)
{
    while(1);
}
