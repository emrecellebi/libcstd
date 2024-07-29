#ifndef STRUTILS_H
#define STRUTILS_H

// #include <stdint.h>				/// int32_t, uint32_t, int64_t, uint64_t

void* str2memcpy(void* dest, const char* src, size_t n);
char* mem2strcpy(char* dest, const void* src, size_t n, size_t nmax);
char* strrealloc(char* str, const char* newstr);
const char* startswith(const char* str, const char* prefix);
const char* startswith_no_case(const char* str, const char* prefix);
const char* endswith(const char* str, const char* postfix);
void strrep(char* str, int find, int replace);
void strrem(char* str, int rem);
char* ul_next_string(char* p, char* end);
const char* skip_space(const char* str);
const char* skip_blank(const char* str);
size_t rtrim_whitespace(unsigned char* str);
size_t ltrim_whitespace(unsigned char* str);
size_t __normalize_whitespace(const unsigned char* src, size_t sz, unsigned char* dst, size_t len);
size_t normalize_whitespace(unsigned char* str);
int isdigit_strend(const char* str, const char** end);
int isxdigit_strend(const char* str, const char** end);
int parse_range(const char* str, int* lower, int* upper, int def);
const char* next_path_segment(const char* str, size_t* sz);
int streq_paths(const char* a, const char* b);
char* ul_strchr_escaped(const char* s, int c);
int ul_stralnumcmp(const char* p1, const char* p2);
size_t strcspn_escaped(const char* s, const char* reject);
int do_scale_by_power(uintmax_t* x, int base, int power);
int parse_size(const char* str, uintmax_t* res, int* power);
int strtosize(const char* str, uintmax_t* res);
int parse_switch(const char* arg, const char* errmesg, ...);
int ul_strtos64(const char* str, int64_t* num, int base);
int ul_strtou64(const char* str, uint64_t* num, int base);
int ul_strtos32(const char* str, int32_t* num, int base);
int ul_strtou32(const char* str, uint32_t* num, int base);
int64_t str2num_or_err(const char* str, int base, const char* errmesg, int64_t low, int64_t up);
uint64_t str2unum_or_err(const char* str, int base, const char* errmesg, uint64_t up);

#define isdigit_string(str) isdigit_strend(str, NULL)
#define isxdigit_string(str) isxdigit_strend(str, NULL)

#endif