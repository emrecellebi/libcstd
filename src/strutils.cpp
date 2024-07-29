#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <ctype.h>
#include <errno.h>
// #include <sys/stat.h>
#include <string.h>
#include <locale.h>
// #include <strings.h>
// #include <assert.h>

// #include "strutils.h"
#include "extra.h"

int STRTOXX_EXIT_CODE = EXIT_FAILURE;

void* str2memcpy(void* dest, const char* src, size_t n)
{
	size_t bytes = strlen(src) + 1;
	if(bytes > n) bytes = n;
	memcpy(dest, src, bytes);
	
	return dest;
}

char* mem2strcpy(char* dest, const void* src, size_t n, size_t nmax)
{
	if(n + 1 > nmax) n = nmax - 1;
	memset(dest, '\0', nmax);
	memcpy(dest, src, n);
	
	return dest;
}

char* strrealloc(char* str, const char* newstr)
{
	size_t nsz, osz;
	
	if(!str) return newstr ? strdup(newstr) : NULL;
	if(!newstr) return NULL;

	osz = strlen(str);
	nsz = strlen(newstr);
	
	if(nsz > osz)str = (char*)realloc(str, nsz + 1);
	if(str) memcpy(str, newstr, nsz + 1);
	
	return str;
}

const char* startswith(const char* str, const char* prefix)
{
	size_t sz = prefix ? strlen(prefix) : 0;
	if(str && sz && strncmp(str, prefix, sz) == 0)
		return str + sz;
	return NULL;
}

const char* startswith_no_case(const char* str, const char* prefix)
{
	size_t sz = prefix ? strlen(prefix) : 0;
	if(str && sz && strncasecmp(str, prefix, sz) == 0)
		return str + sz;
	return NULL;
}

const char* endswith(const char* str, const char* postfix)
{
	size_t sl = str ? strlen(str) : 0;
	size_t pl = postfix ? strlen(postfix) : 0;

	if(pl == 0) return str + sl;
	if(sl < pl) return NULL;
	if(memcmp(str + sl - pl, postfix, pl) != 0) return NULL;
	
	return str + sl - pl;
}

void strrep(char* str, int find, int replace)
{
	while(str && *str && (str = strchr(str, find)) != NULL)
		*str++ = replace;
}

void strrem(char* str, int rem)
{
	char* tmp;

	if(!str) return;
	for(tmp = str; *str; str++)
		if(*str != rem) *tmp++ = *str;
	*tmp = '\0';
}

char* ul_next_string(char* p, char* end)
{
	char* last;

	if(!p || !end || p >= end)
		return NULL;
	
	for(last = p; p < end; p++)
	{
		if(*last == '\0' && p != last)
			return p;
		last = p;
	}

	return NULL;
}

const char* skip_space(const char* str)
{
	while(isspace(*str)) ++str;
	return str;
}

const char* skip_blank(const char* str)
{
	while(isblank(*str)) ++str;
	return str;
}

size_t rtrim_whitespace(unsigned char* str)
{
	size_t i;

	if(!str) return 0;
	i = strlen((char*)str);
	while(i)
	{
		i--;
		if(!isspace(str[i]))
		{
			i++;
			break;
		}
	}
	str[i] = '\0';
	return i;
}

size_t ltrim_whitespace(unsigned char* str)
{
	size_t len;
	unsigned char* p;

	if(!str) return 0;
	for(p = str; *p && isspace(*p); p++);
	len = strlen((char*)p);
	if(p > str) memmove(str, p, len + 1);

	return len;
}

size_t __normalize_whitespace(const unsigned char* src, size_t sz, unsigned char* dst, size_t len)
{
	size_t i, x = 0;
	int nsp = 0, intext = 0;

	if(!sz) goto done;

	for(i = 0, x = 0; i < sz && x < len - 1;)
	{
		if(isspace(src[i])) nsp++; else nsp = 0, intext = 1;
		if(nsp > 1 || (nsp && !intext)) i++; else dst[x++] = src[i++];
	}
	if(nsp && x > 0) x--;
	
done:
	dst[x] = '\0';
	return x;
}

size_t normalize_whitespace(unsigned char* str)
{
	size_t sz = strlen((char*)str);
	return __normalize_whitespace(str, sz, str, sz + 1);
}

int isdigit_strend(const char* str, const char** end)
{
	const char* p;

	for(p = str; p && *p && isdigit((unsigned char)*p); p++);
	if(end) *end = p;
	
	return p && p > str && !*p;
}

int isxdigit_strend(const char* str, const char** end)
{
	const char* p;

	for(p = str; p && *p && isxdigit((unsigned char)*p); p++);
	if(end) *end = p;

	return p && p > str && !*p;
}

int parse_range(const char* str, int* lower, int* upper, int def)
{
	char* end = NULL;

	if(!str) return 0;
	*upper = *lower = def;
	if(*str == ':')
	{
		str++;
		*upper = strtol(str, &end, 10);
		if(!end || *end || end == str) return -1;
	}
	else
	{
		*upper = *lower = strtol(str, &end, 10);
		if(!end || end == str) return -1;
		if(*end == ':' && !*(end + 1))
			*upper = def;
		else if(*end == '-' || *end == ':')
		{
			str = end + 1;
			end = NULL;
			*upper = strtol(str, &end, 10);
			if(!end || *end || end == str) return -1;
		}
	}
	return 0;
}

const char* next_path_segment(const char* str, size_t* sz)
{
	const char* start;
	const char* p;

	start = str;
	*sz = 0;
	while(start && *start == '/' && *(start + 1) == '/') start++;
	if(!start || !*start) return NULL;
	for(*sz = 1, p = start + 1; *p && *p != '/'; p++) (*sz)++;

	return start;
}

int streq_paths(const char* a, const char* b)
{
	while(a && b)
	{
		size_t a_sz, b_sz;
		const char* a_seg = next_path_segment(a, &a_sz);
		const char* b_seg = next_path_segment(b, &b_sz);
		
		/** stderr **/
		// printf("A >>> (%zu) '%s'\n", a_sz, a_seg);
		// printf("B >>> (%zu) '%s'\n", b_sz, b_seg);

		if(a_sz + b_sz == 0) return 1;
		if(a_sz + b_sz == 1 && ((a_seg && *a_seg == '/') || (b_seg && *b_seg == '/'))) return 1;
		if(!a_seg || !b_seg) break;
		if(a_sz != b_sz || strncmp(a_seg, b_seg, a_sz) != 0) break;

		a = a_seg + a_sz;
		b = b_seg + b_sz;
	};

	return 0;
}

char* ul_strchr_escaped(const char* s, int c)
{
	char* p;
	int esc = 0;

	for(p = (char*)s; p && *p; p++)
	{
		if(!esc && *p == '\\')
		{
			esc = 1;
			continue;
		}
		if(*p == c && (!esc || c == '\\')) return p;
		esc = 0;
	}

	return NULL;
}

int ul_stralnumcmp(const char* p1, const char* p2)
{
	const unsigned char* s1 = (const unsigned char*)p1;
	const unsigned char* s2 = (const unsigned char*)p2;
	unsigned char c1, c2;

	do
	{
		do
		{
			c1 = (unsigned char)*s1++;
		}while(c1 != '\0' && !isalnum((unsigned int)c1));

		do
		{
			c2 = (unsigned char)*s2++;
		}while(c2 != '\0' && !isalnum((unsigned int)c2));

		if(c1 != '\0') c1 = tolower(c1);
		if(c2 != '\0') c2 = tolower(c2);
		if(c1 == '\0') return c1 - c2;
	}while(c1 == c2);

	return c1 - c2;
}

size_t strcspn_escaped(const char* s, const char* reject)
{
	int escaped = 0;
	int n;
	
	for(n = 0; s[n]; n++)
	{
		if(escaped)
			escaped = 0;
		else if(s[n] == '\\')
			escaped = 1;
		else if(strchr(reject, s[n]))
			break;
	}
	return n - escaped;
}

int do_scale_by_power(uintmax_t* x, int base, int power)
{
	while(power--)
	{
		if(UINTMAX_MAX / base < *x)
			return -ERANGE;
		*x *= base;
	}
	return 0;
}

/*
 * strtosize() - convert string to size (uintmax_t).
 *
 * Supported suffixes:
 *
 * XiB or X for 2^N
 *     where X = {K,M,G,T,P,E,Z,Y}
 *        or X = {k,m,g,t,p,e}  (undocumented for backward compatibility only)
 * for example:
 *		10KiB	= 10240
 *		10K	= 10240
 *
 * XB for 10^N
 *     where X = {K,M,G,T,P,E,Z,Y}
 * for example:
 *		10KB	= 10000
 *
 * The optional 'power' variable returns number associated with used suffix
 * {K,M,G,T,P,E,Z,Y}  = {1,2,3,4,5,6,7,8}.
 *
 * The function also supports decimal point, for example:
 *              0.5MB   = 500000
 *              0.5MiB  = 512000
 *
 * Note that the function does not accept numbers with '-' (negative sign)
 * prefix.
 */
int parse_size(const char* str, uintmax_t* res, int* power)
{
	const char* p;
	char* end;
	uintmax_t x, frac = 0;
	int base = 1024, rc = 0, pwr = 0, frac_zeros = 0;

	static const char* suf  = "KMGTPEZY";
	static const char* suf2 = "kmgtpezy";
	const char* sp;

	*res = 0;

	if(!str || !*str)
	{
		rc = -EINVAL;
		goto err;
	}

	p = str;
	while(isspace((unsigned char)*p)) p++;
	if(*p == '-')
	{
		rc = -EINVAL;
		goto err;
	}

	errno = 0, end = NULL;
	x = strtoumax(str, &end, 0);

	if(end == str || (errno != 0 && (x == UINTMAX_MAX || x == 0)))
	{
		rc = errno ? -errno : -EINVAL;
		goto err;
	}
	if(!end || !*end)
		goto done;
	p = end;

check_suffix:
	if(*(p + 1) == 'i' && (*(p + 2) == 'B' || *(p + 2) == 'b') && !*(p + 3))
		base = 1024;
	else if((*(p + 1) == 'B' || *(p + 1) == 'b') && !*(p + 2))
		base = 1000;
	else if(*(p + 1))
	{
		struct lconv const *l = localeconv();
		const char* dp = l ? l->decimal_point : NULL;
		size_t dpsz = dp ? strlen(dp) : 0;

		if(frac == 0 && *p && dp && strncmp(dp, p, dpsz) == 0)
		{
			const char* fstr = p + dpsz;

			for(p = fstr; *p == '0'; p++)
				frac_zeros++;
			fstr = p;
			if(isdigit(*fstr))
			{
				errno = 0, end = NULL;
				frac = strtoumax(fstr, &end, 0);
				if(end == fstr || (errno != 0 && (frac == UINTMAX_MAX || frac == 0)))
				{
					rc = errno ? -errno : -EINVAL;
					goto err;
				}
			}
			else end = (char *)p;

			if(frac && (!end  || !*end))
			{
				rc = -EINVAL;
				goto err;
			}
			p = end;
			goto check_suffix;
		}
		rc = -EINVAL;
		goto err;
	}

	sp = strchr(suf, *p);
	if(sp)
		pwr = (sp - suf) + 1;
	else
	{
		sp = strchr(suf2, *p);
		if(sp)
			pwr = (sp - suf2) + 1;
		else
		{
			rc = -EINVAL;
			goto err;
		}
	}

	rc = do_scale_by_power(&x, base, pwr);
	if(power)
		*power = pwr;
	if (frac && pwr)
	{
		int i;
		uintmax_t frac_div = 10, frac_poz = 1, frac_base = 1;
		
		do_scale_by_power(&frac_base, base, pwr);

		/* maximal divisor for last digit (e.g. for 0.05 is
		 * frac_div=100, for 0.054 is frac_div=1000, etc.)
		 *
		 * Reduce frac if too large.
		 */
		while(frac_div < frac)
		{
			if(frac_div <= UINTMAX_MAX / 10)
				frac_div *= 10;
			else
				frac /= 10;
		}

		/* 'frac' is without zeros (5 means 0.5 as well as 0.05) */
		for(i = 0; i < frac_zeros; i++)
		{
			if(frac_div <= UINTMAX_MAX/10)
				frac_div *= 10;
			else
				frac /= 10;
		}

		/*
		 * Go backwardly from last digit and add to result what the
		 * digit represents in the frac_base. For example 0.25G
		 *
		 *  5 means 1GiB / (100/5)
		 *  2 means 1GiB / (10/2)
		 */
		do
		{
			unsigned int seg = frac % 10;
			uintmax_t seg_div = frac_div / frac_poz;

			frac /= 10;
			frac_poz *= 10;

			if(seg && seg_div / seg)
				x += frac_base / (seg_div / seg);
		}while(frac);
	}
done:
	*res = x;
err:
	if(rc < 0)
		errno = -rc;
	return rc;
}

int strtosize(const char* str, uintmax_t* res)
{
	return parse_size(str, res, NULL);
}

int parse_switch(const char* arg, const char* errmesg, ...)
{
	const char* a;
	const char* b;
	va_list ap;

	va_start(ap, errmesg);
	do
	{
		a = va_arg(ap, char*);
		if(!a) break;
		
		b = va_arg(ap, char*);
		if(!b) break;

		if(strcmp(arg, a) == 0)
		{
			va_end(ap);
			return 1;
		}

		if(strcmp(arg, b) == 0)
		{
			va_end(ap);
			return 0;
		}
	}while(1);
	va_end(ap);

	errx(STRTOXX_EXIT_CODE, "%s: '%s'", errmesg, arg);
	
	return 0;
}

int ul_strtos64(const char* str, int64_t* num, int base)
{
	char* end = NULL;

	if(str == NULL || *str == '\0')
		return -(errno = EINVAL);

	errno = 0;
	*num = (int64_t)strtoimax(str, &end, base);

	if(errno != 0)
		return -errno;
	if(str == end || (end && *end))
		return -(errno = EINVAL);
	return 0;
}

int ul_strtou64(const char* str, uint64_t* num, int base)
{
	char* end = NULL;
	int64_t tmp;

	if(str == NULL || *str == '\0')
		return -(errno = EINVAL);

	errno = 0;
	tmp = (int64_t)strtoimax(str, &end, base);
	if(tmp < 0)
		errno = ERANGE;
	else
	{
		errno = 0;
		*num = strtoumax(str, &end, base);
	}

	if(errno != 0)
		return -errno;
	if(str == end || (end && *end))
		return -(errno = EINVAL);
	return 0;
}

int ul_strtos32(const char* str, int32_t* num, int base)
{
	int64_t tmp;
	int rc;

	rc = ul_strtos64(str, &tmp, base);
	if(rc == 0 && (tmp < INT32_MIN || tmp > INT32_MAX))
		rc = -(errno = ERANGE);
	if(rc == 0)
		*num = (int32_t)tmp;
	return rc;
}

int ul_strtou32(const char* str, uint32_t* num, int base)
{
	uint64_t tmp;
	int rc;

	rc = ul_strtou64(str, &tmp, base);
	if(rc == 0 && tmp > UINT32_MAX)
		rc = -(errno = ERANGE);
	if(rc == 0)
		*num = (uint32_t) tmp;
	return rc;
}

int64_t str2num_or_err(const char* str, int base, const char* errmesg, int64_t low, int64_t up)
{
	int64_t num = 0;
	int rc;

	rc = ul_strtos64(str, &num, base);
	if(rc == 0 && ((low && num < low) || (up && num > up)))
		rc = -(errno = ERANGE);

	if(rc)
	{
		if(errno == ERANGE)
			err(STRTOXX_EXIT_CODE, "%s: '%s'", errmesg, str);
		errx(STRTOXX_EXIT_CODE, "%s: '%s'", errmesg, str);
	}
	return num;
}

uint64_t str2unum_or_err(const char* str, int base, const char* errmesg, uint64_t up)
{
	uint64_t num = 0;
	int rc;

	rc = ul_strtou64(str, &num, base);
	if(rc == 0 && (up && num > up))
		rc = -(errno = ERANGE);

	if(rc)
	{
		if(errno == ERANGE)
			err(STRTOXX_EXIT_CODE, "%s: '%s'", errmesg, str);
		errx(STRTOXX_EXIT_CODE, "%s: '%s'", errmesg, str);
	}
	return num;
}