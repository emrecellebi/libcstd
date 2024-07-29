#ifndef CRC32C_H
#define CRC32C_H

uint32_t crc32c(uint32_t crc, const void* buf, size_t size);
uint32_t ul_crc32c_exclude_offset(uint32_t crc, const unsigned char* buf, size_t size, size_t exclude_off, size_t exclude_len);

#endif