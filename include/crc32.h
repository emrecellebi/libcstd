#ifndef CRC32_H
#define CRC32_H

uint32_t ul_crc32(uint32_t seed, const uint8_t* buf, size_t len);
uint32_t ul_crc32_exclude_offset(uint32_t seed, const uint8_t* buf, size_t len, size_t exclude_off, size_t exclude_len);

#endif