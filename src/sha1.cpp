#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "sha1.h"

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15]^block->l[(i+2)&15]^block->l[i&15],1))

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#define blk0(i) block->l[i]
#else
	#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00)|(rol(block->l[i],8)&0x00FF00FF))
#endif

#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

void ul_SHA1Init(UL_SHA1_CTX* ctx)
{
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xC3D2E1F0;
	ctx->count[0] = ctx->count[1] = 0;
}

void ul_SHA1Update(UL_SHA1_CTX* ctx, const unsigned char* data, uint32_t len)
{
	uint32_t i;
	uint32_t j;

	j = ctx->count[0];
	if((ctx->count[0] += len << 3) < j)
		ctx->count[1]++;
	ctx->count[1] += (len >> 29);
	j = (j >> 3) & 63;
	if((j + len) > 63)
	{
		memcpy(&ctx->buffer[j], data, (i = 64 - j));
		ul_SHA1Transform(ctx->state, ctx->buffer);
		for(; i + 63 < len; i += 64)
			ul_SHA1Transform(ctx->state, &data[i]);
		j = 0;
	}else
		i = 0;
	memcpy(&ctx->buffer[j], &data[i], len - i);
}

void ul_SHA1Final(unsigned char digest[20], UL_SHA1_CTX* ctx)
{
	unsigned i;
	unsigned char finalcount[8];
	unsigned char c;

#if 0
	unsigned char *fcp = &finalcount[8];
	for(i = 0; i < 2; i++)
	{
		uint32_t t = ctx->count[i];
		int j;
		for(j = 0; j < 4; t >>= 8, j++)
			*--fcp = (unsigned char)t
	}
#else
	for(i = 0; i < 8; i++)
	{
		finalcount[i] = (unsigned char)((ctx->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);	/* Endian independent */
	}
#endif
	c = 0200;
	ul_SHA1Update(ctx, &c, 1);
	while((ctx->count[0] & 504) != 448)
	{
		c = 0000;
		ul_SHA1Update(ctx, &c, 1);
	}
	ul_SHA1Update(ctx, finalcount, 8);	/* Should cause a SHA1Transform() */
	for(i = 0; i < 20; i++)
		digest[i] = (unsigned char)((ctx->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
	memset(ctx, '\0', sizeof(*ctx));
	memset(&finalcount, '\0', sizeof(finalcount));
}

void ul_SHA1Transform(uint32_t state[5], const unsigned char buffer[64])
{
	uint32_t a, b, c, d, e;

	union CHAR64LONG16
	{
		unsigned char c[64];
		uint32_t l[16];
	};

	CHAR64LONG16 block[1];
	memcpy(block, buffer, 64);
	
	// CHAR64LONG16 *block = (const CHAR64LONG16 *)buffer;

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	
	R0(a, b, c, d, e, 0);
	R0(e, a, b, c, d, 1);
	R0(d, e, a, b, c, 2);
	R0(c, d, e, a, b, 3);
	R0(b, c, d, e, a, 4);
	R0(a, b, c, d, e, 5);
	R0(e, a, b, c, d, 6);
	R0(d, e, a, b, c, 7);
	R0(c, d, e, a, b, 8);
	R0(b, c, d, e, a, 9);
	R0(a, b, c, d, e, 10);
	R0(e, a, b, c, d, 11);
	R0(d, e, a, b, c, 12);
	R0(c, d, e, a, b, 13);
	R0(b, c, d, e, a, 14);
	R0(a, b, c, d, e, 15);
	R1(e, a, b, c, d, 16);
	R1(d, e, a, b, c, 17);
	R1(c, d, e, a, b, 18);
	R1(b, c, d, e, a, 19);
	R2(a, b, c, d, e, 20);
	R2(e, a, b, c, d, 21);
	R2(d, e, a, b, c, 22);
	R2(c, d, e, a, b, 23);
	R2(b, c, d, e, a, 24);
	R2(a, b, c, d, e, 25);
	R2(e, a, b, c, d, 26);
	R2(d, e, a, b, c, 27);
	R2(c, d, e, a, b, 28);
	R2(b, c, d, e, a, 29);
	R2(a, b, c, d, e, 30);
	R2(e, a, b, c, d, 31);
	R2(d, e, a, b, c, 32);
	R2(c, d, e, a, b, 33);
	R2(b, c, d, e, a, 34);
	R2(a, b, c, d, e, 35);
	R2(e, a, b, c, d, 36);
	R2(d, e, a, b, c, 37);
	R2(c, d, e, a, b, 38);
	R2(b, c, d, e, a, 39);
	R3(a, b, c, d, e, 40);
	R3(e, a, b, c, d, 41);
	R3(d, e, a, b, c, 42);
	R3(c, d, e, a, b, 43);
	R3(b, c, d, e, a, 44);
	R3(a, b, c, d, e, 45);
	R3(e, a, b, c, d, 46);
	R3(d, e, a, b, c, 47);
	R3(c, d, e, a, b, 48);
	R3(b, c, d, e, a, 49);
	R3(a, b, c, d, e, 50);
	R3(e, a, b, c, d, 51);
	R3(d, e, a, b, c, 52);
	R3(c, d, e, a, b, 53);
	R3(b, c, d, e, a, 54);
	R3(a, b, c, d, e, 55);
	R3(e, a, b, c, d, 56);
	R3(d, e, a, b, c, 57);
	R3(c, d, e, a, b, 58);
	R3(b, c, d, e, a, 59);
	R4(a, b, c, d, e, 60);
	R4(e, a, b, c, d, 61);
	R4(d, e, a, b, c, 62);
	R4(c, d, e, a, b, 63);
	R4(b, c, d, e, a, 64);
	R4(a, b, c, d, e, 65);
	R4(e, a, b, c, d, 66);
	R4(d, e, a, b, c, 67);
	R4(c, d, e, a, b, 68);
	R4(b, c, d, e, a, 69);
	R4(a, b, c, d, e, 70);
	R4(e, a, b, c, d, 71);
	R4(d, e, a, b, c, 72);
	R4(c, d, e, a, b, 73);
	R4(b, c, d, e, a, 74);
	R4(a, b, c, d, e, 75);
	R4(e, a, b, c, d, 76);
	R4(d, e, a, b, c, 77);
	R4(c, d, e, a, b, 78);
	R4(b, c, d, e, a, 79);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	a = b = c = d = e = 0;
	memset(block, '\0', sizeof(block));
}

void ul_SHA1(char* hash_out, const char* str, unsigned len)
{
	UL_SHA1_CTX ctx;
	unsigned int ii;

	ul_SHA1Init(&ctx);
	for(ii = 0; ii < len; ii += 1)
		ul_SHA1Update(&ctx, (const unsigned char *)str + ii, 1);
	ul_SHA1Final((unsigned char *)hash_out, &ctx);
	hash_out[20] = '\0';
}