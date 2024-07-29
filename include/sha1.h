#ifndef SHA1_H
#define SHA1_H

#define UL_SHA1LENGTH 20

struct UL_SHA1_CTX
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
};

void ul_SHA1Init(UL_SHA1_CTX* ctx);	/* SHA1Init - Initialize new context */
void ul_SHA1Update(UL_SHA1_CTX* ctx, const unsigned char* data, uint32_t len); /* SHA1Update - Update context */
void ul_SHA1Final(unsigned char digest[20], UL_SHA1_CTX* ctx);				/* SHA1Final - Final context */
void ul_SHA1Transform(uint32_t state[5], const unsigned char buffer[64]);	/* SHA1Transform - Transform context */
void ul_SHA1(char* hash_out, const char* str, unsigned len);

#endif