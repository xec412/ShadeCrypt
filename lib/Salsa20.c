#include "Salsa20.h"

/*----------------------------------------------
* Macros
------------------------------------------------*/
#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define U8TO32_LITTLE(p) \
    (((uint32_t)((p)[0])      ) | \
     ((uint32_t)((p)[1]) <<  8) | \
     ((uint32_t)((p)[2]) << 16) | \
     ((uint32_t)((p)[3]) << 24))

#define U32TO8_LITTLE(p, v) \
    do { \
        (p)[0] = (uint8_t)((v)      ); \
        (p)[1] = (uint8_t)((v) >>  8); \
        (p)[2] = (uint8_t)((v) >> 16); \
        (p)[3] = (uint8_t)((v) >> 24); \
    } while (0)

/*----------------------------------------------
* Salsa20 Quarter Round
* QR(a,b,c,d) modifies b,c,d based on a
------------------------------------------------*/
#define QUARTERROUND(a, b, c, d) \
    b ^= ROTL32(a + d,  7); \
    c ^= ROTL32(b + a,  9); \
    d ^= ROTL32(c + b, 13); \
    a ^= ROTL32(d + c, 18);

/*----------------------------------------------
* Salsa20 Row Round (4 quarter rounds)
------------------------------------------------*/
static void salsa20_rowround(uint32_t state[16]) {
    QUARTERROUND(state[0], state[1], state[2], state[3]);
    QUARTERROUND(state[5], state[6], state[7], state[4]);
    QUARTERROUND(state[10], state[11], state[8], state[9]);
    QUARTERROUND(state[15], state[12], state[13], state[14]);
}

/*----------------------------------------------
* Salsa20 Column Round (4 quarter rounds)
------------------------------------------------*/
static void salsa20_columnround(uint32_t state[16]) {
    QUARTERROUND(state[0], state[4], state[8], state[12]);
    QUARTERROUND(state[5], state[9], state[13], state[1]);
    QUARTERROUND(state[10], state[14], state[2], state[6]);
    QUARTERROUND(state[15], state[3], state[7], state[11]);
}

/*----------------------------------------------
* Salsa20 Double Round (column + row)
------------------------------------------------*/
static void salsa20_doubleround(uint32_t state[16]) {
    salsa20_columnround(state);
    salsa20_rowround(state);
}

/*----------------------------------------------
* Salsa20 Core - 20 rounds (10 double rounds)
------------------------------------------------*/
static void salsa20_core(uint32_t output[16], const uint32_t input[16]) {
    uint32_t x[16];
    int i;

    // Copy input to working state
    for (i = 0; i < 16; i++) {
        x[i] = input[i];
    }

    // 10 double rounds = 20 rounds total
    for (i = 0; i < 10; i++) {
        salsa20_doubleround(x);
    }

    // Add input to output (diffusion)
    for (i = 0; i < 16; i++) {
        output[i] = x[i] + input[i];
    }
}

/*----------------------------------------------
* Initialize Salsa20 Context
* key: 32 bytes (256-bit)
* nonce: 8 bytes (64-bit)
------------------------------------------------*/
void salsa20_init(salsa20_ctx* ctx, const uint8_t* key, const uint8_t* nonce) {
    const uint8_t* constants = (const uint8_t*)"expand 32-byte k";

    // State layout (Salsa20 specification):
    // [const] [key  ] [key  ] [key  ]
    // [key  ] [const] [nonce] [nonce]
    // [count] [count] [const] [key  ]
    // [key  ] [key  ] [key  ] [const]

    ctx->state[0] = U8TO32_LITTLE(constants + 0);
    ctx->state[1] = U8TO32_LITTLE(key + 0);
    ctx->state[2] = U8TO32_LITTLE(key + 4);
    ctx->state[3] = U8TO32_LITTLE(key + 8);
    ctx->state[4] = U8TO32_LITTLE(key + 12);
    ctx->state[5] = U8TO32_LITTLE(constants + 4);
    ctx->state[6] = U8TO32_LITTLE(nonce + 0);
    ctx->state[7] = U8TO32_LITTLE(nonce + 4);
    ctx->state[8] = 0;  // Counter low
    ctx->state[9] = 0;  // Counter high
    ctx->state[10] = U8TO32_LITTLE(constants + 8);
    ctx->state[11] = U8TO32_LITTLE(key + 16);
    ctx->state[12] = U8TO32_LITTLE(key + 20);
    ctx->state[13] = U8TO32_LITTLE(key + 24);
    ctx->state[14] = U8TO32_LITTLE(key + 28);
    ctx->state[15] = U8TO32_LITTLE(constants + 12);

    ctx->position = 64; // Force keystream generation on first use
}

/*----------------------------------------------
* Set Counter (for seeking in stream)
------------------------------------------------*/
void salsa20_set_counter(salsa20_ctx* ctx, uint64_t counter) {
    ctx->state[8] = (uint32_t)counter;
    ctx->state[9] = (uint32_t)(counter >> 32);
    ctx->position = 64; // Force keystream regeneration
}

/*----------------------------------------------
* Generate Keystream Block
------------------------------------------------*/
void salsa20_block(salsa20_ctx* ctx, uint8_t* output) {
    uint32_t block[16];
    int i;

    // Generate keystream block
    salsa20_core(block, ctx->state);

    // Convert to bytes
    for (i = 0; i < 16; i++) {
        U32TO8_LITTLE(output + (i * 4), block[i]);
    }

    // Increment counter
    ctx->state[8]++;
    if (ctx->state[8] == 0) {
        ctx->state[9]++;
    }
}

/*----------------------------------------------
* Encrypt/Decrypt Data
* Same operation (XOR with keystream)
------------------------------------------------*/
void salsa20_crypt(salsa20_ctx* ctx, uint8_t* data, size_t length) {
    size_t i;

    for (i = 0; i < length; i++) {
        // Generate new keystream block if needed
        if (ctx->position >= 64) {
            salsa20_block(ctx, ctx->keystream);
            ctx->position = 0;
        }

        // XOR data with keystream
        data[i] ^= ctx->keystream[ctx->position];
        ctx->position++;
    }
}
