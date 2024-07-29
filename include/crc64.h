#ifndef CRC64_H
#define CRC64_H

uint64_t ul_crc64_ecma(const unsigned char* ptr, size_t num_bytes)
uint64_t ul_crc64_we(const unsigned char* ptr, size_t num_bytes);
uint64_t ul_update_crc64(uint64_t crc, unsigned char c);

#endif