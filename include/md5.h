#ifndef MD5_H
#define MD5_H

#define UL_MD5LENGTH 16

struct UL_MD5Context
{
	uint32_t buf[4];
	uint32_t bits[2];
	unsigned char in[64];
};

typedef UL_MD5Context UL_MD5_CTX;

void ul_MD5Init(struct UL_MD5Context* ctx);
void ul_MD5Update(struct UL_MD5Context* ctx, unsigned char const* buf, unsigned len);
void ul_MD5Final(unsigned char digest[UL_MD5LENGTH], struct UL_MD5Context* ctx);
void ul_MD5Transform(uint32_t buf[4], uint32_t const in[16]);
void ul_MD5(unsigned char hash_out[UL_MD5LENGTH], const unsigned char* str, size_t len);

#endif