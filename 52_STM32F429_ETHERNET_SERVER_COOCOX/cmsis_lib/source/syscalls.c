/**************************************************************************//*****
 * @file     stdio.c
 * @brief    Implementation of newlib syscall
 ********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#undef errno
extern int errno;
extern int  _end;

int __attribute__((weak)) fputc(int ch, FILE *f) {
	/* Return 0 */
	return 0;
}

__attribute__ ((used))
caddr_t _sbrk (int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;

	heap += incr;

	return (caddr_t) prev_heap;
}

__attribute__ ((used))
int link(char *old, char *new) {
	return -1;
}

__attribute__ ((used))
int _close(int file) {
	return -1;
}

__attribute__ ((used))
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

__attribute__ ((used))
int _isatty(int file) {
	return 1;
}

__attribute__ ((used))
int _lseek(int file, int ptr, int dir) {
	return 0;
}

__attribute__ ((used))
int _read(int file, char *ptr, int len) {
	return 0;
}

__attribute__ ((used))
int _write(int file, char *ptr, int len) {
	int i;
	/* Go through all characters */
	for (i = 0; i < len; i++) {
		/* Call fputc standard function */
		if (fputc(ptr[i], NULL) != ptr[i]) {
			break;
		}
	}
	return len;
}

__attribute__ ((used))
void abort(void) {
	/* Abort called */
	while (1);
}

/* --------------------------------- End Of File ------------------------------ */
