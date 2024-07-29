#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

char* str_append(char* dst, char* src);
char* hex_append(char* dst, uint8_t* src, int len);
void skip_char(char sc, char** buf);
void find_char(char fc, char** buf);
void hexdump(FILE* fs, uint8_t* buf, int len, int space);
int hex2bin(char* hex, uint8_t* bin);
char* parse_hex(char* buf, uint8_t* dst, int len);
void dump_hex(uint8_t* src, int len);
void dump_bytes_to_file(char* fname, uint8_t* d, int len);

#endif