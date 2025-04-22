/*
 * syscalls_override.c
 *
 *  Created on: 02 августа 2024 г.
 *      Author: Vladimir
 */

/*

Before gcc arm 10 everything is ok.
After gcc arm 13 issues happened:

gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-closer.o):
in function `_close_r': closer.c:(.text._close_r+0xc): warning: _close is not implemented and will
always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-fstatr.o):
in function `_fstat_r': fstatr.c:(.text._fstat_r+0xe): warning: _fstat is not implemented and will
always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-signalr.o):
in function `_getpid_r': signalr.c:(.text._getpid_r+0x0): warning: _getpid is not implemented and
will always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-isattyr.o):
in function `_isatty_r': isattyr.c:(.text._isatty_r+0xc): warning: _isatty is not implemented and
will always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-signalr.o):
in function `_kill_r': signalr.c:(.text._kill_r+0xe): warning: _kill is not implemented and will
always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-lseekr.o):
in function `_lseek_r': lseekr.c:(.text._lseek_r+0x10): warning: _lseek is not implemented and will
always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-readr.o):
in function `_read_r': readr.c:(.text._read_r+0x10): warning: _read is not implemented and will
always fail
gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/bin/ld.exe:
gcc/arm-none-eabi/13.2.1/thumb/v7e-m+fp/hard\libc_nano.a(libc_a-writer.o):
in function `_write_r': writer.c:(.text._write_r+0x10): warning: _write is not implemented and will
always fail

https://community.silabs.com/s/question/0D5Vm0000047to9KAA/linker-errors-on-update-to-gnu-arm-v1221?language=es
https://stackoverflow.com/questions/73742774/gcc-arm-none-eabi-11-3-is-not-implemented-and-will-always-fail
https://github.com/contiki-ng/contiki-ng/issues/2823
https://interrupt.memfault.com/blog/boostrapping-libc-with-newlib

Here stubs are defined for:

_close
_fstat
_getpid
_isatty
_kill
_lseek
_read
_write

*/

#include <sys/stat.h>

int _close(int file) {
    (void)file;
    return -1;
}

int _fstat(int file, struct stat* st) {
    (void)file;
    (void)st;
    return 0;
}

int _getpid(void) { return -1; }

int _isatty(int file) {
    (void)file;
    return 1;
}

void _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int fd, char* buf, int count) {
    (void)fd;
    (void)buf;
    (void)count;
    return 0;
}

int _write(int fd, char* buf, int count) {
    (void)fd;
    (void)buf;
    (void)count;
    return 0;
}