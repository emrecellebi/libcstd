#ifndef EXTRAS_H
#define EXTRAS_H

/// cpp file olamayan header dosykarı buraya ekle

#ifdef HAVE_GETEXECNAME
#define program_invocation_short_name prog_inv_sh_nm_from_file(getexecname(), 0)
#else
#define program_invocation_short_name prog_inv_sh_nm_from_file(strdup(__FILE__), 1)
#endif

#define err(E, FMT...) errmsg(1, E, 1, FMT)
#define warn(FMT...) errmsg(0, 0, 1, FMT)
#define errx(E, FMT...) errmsg(1, E, 0, FMT)
#define warnx(FMT...) errmsg(0, 0, 0, FMT)

char* prog_inv_sh_nm_from_file(char* f, char stripext);
void errmsg(char doexit, int excode, char adderr, const char* fmt, ...);

#endif