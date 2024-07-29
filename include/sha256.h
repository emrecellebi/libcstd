#ifndef SHA256_H
#define SHA256_H

#define UL_SHA256LENGTH		32

struct sha256 {
	uint64_t len;
	uint32_t h[8];
	uint8_t buf[64];
};

void sha256_init(struct sha256* s);
void sha256_update(struct sha256* s, const void* m, unsigned long len);
void sha256_sum(struct sha256* s, uint8_t* md);
void ul_SHA256(unsigned char hash_out[UL_SHA256LENGTH], const unsigned char* str, size_t len);

#endif