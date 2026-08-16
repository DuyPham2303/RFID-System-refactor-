/* Minimal system call implementations for newlib
 * Provides `_sbrk` and basic syscall stubs used by the C library.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <fcntl.h>

/* Provide _sbrk for dynamic memory allocation (malloc) */
caddr_t _sbrk(int incr)
{
    extern char _end; /* Provided by linker script */
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == NULL)
    {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;
    heap_end += incr;

    return (caddr_t)prev_heap_end;
}

/* Minimal syscall stubs to satisfy the linker and newlib requirements. */
int _open(const char *name, int flags, int mode)
{
    (void)name;
    (void)flags;
    (void)mode;
    errno = ENOSYS;
    return -1;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

off_t _lseek(int file, off_t offset, int whence)
{
    (void)file;
    (void)offset;
    (void)whence;
    return 0;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, const char *ptr, int len)
{
    (void)file;
    (void)ptr;
    return len;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

void _exit(int status)
{
    (void)status;
    while (1)
    {
        /* Loop forever */
    }
}

pid_t _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

/* Provide weak empty _init/_fini for libc constructor compatibility. */
void _init(void) __attribute__((weak));
void _init(void)
{
}

void _fini(void) __attribute__((weak));
void _fini(void)
{
}