/* error.c -- handles printing warnings and errors
 *
 * Copyright (C) 2017 Alex White <internetunexplorer@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */ 

#ifndef ERROR_C_
#define ERROR_C_

#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

int errmsg(int errnum, const char *format, ...) {
	va_list args;
	fprintf(stderr, "error: ");
	if(format != NULL) {
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
	}
	if(errnum) {
		if(format != NULL)
			fprintf(stderr, ": ");
		fprintf(stderr, "%s", strerror(errnum));
	}
	fprintf(stderr, "\n");
	return errnum;
}

#endif
