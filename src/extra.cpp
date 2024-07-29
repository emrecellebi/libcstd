#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "extra.h"

char prog_inv_sh_nm_buf[256];

char* prog_inv_sh_nm_from_file(char* f, char stripext)
{
	char* t;

	if((t = strrchr(f, '/')) != NULL)
		t++;
	else
		t = f;

	strncpy(prog_inv_sh_nm_buf, t, sizeof(prog_inv_sh_nm_buf) - 1);
	prog_inv_sh_nm_buf[sizeof(prog_inv_sh_nm_buf) - 1] = '\0';

	if(stripext && (t = strrchr(prog_inv_sh_nm_buf, '.')) != NULL)
		*t = '\0';

	return prog_inv_sh_nm_buf;
}

void errmsg(char doexit, int excode, char adderr, const char* fmt, ...) /// __attribute__ ((__format__ (__printf__, 4, 5)))
{
	fprintf(stderr, "%s: ", program_invocation_short_name);
	if(fmt != NULL)
	{
		va_list argp;
		va_start(argp, fmt);
		vfprintf(stderr, fmt, argp);
		va_end(argp);
		if(adderr)
			fprintf(stderr, ": ");
	}
	if(adderr)
		fprintf(stderr, "%m");
	fprintf(stderr, "\n");
	if(doexit)
		exit(excode);
}