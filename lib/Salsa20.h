#pragma once

#include <stdint.h>
#include <string.h>

/*----------------------------------------------
* Salsa20 Core Algorithm Header
* Based on D.J. Bernstein's specification
* https://cr.yp.to/snuffle/spec.pdf
------------------------------------------------*/

#define SALSA20_KEY_SIZE    32  // 256-bit key
#define SALSA20_NONCE_SIZE  8   // 64-bit nonce
#define SALSA20_BLOCK_SIZE  64  // 512-bit block

/*----------------------------------------------
* Salsa20 Context Structure
------------------------------------------------*/
typedef struct {
    uint32_t state[16];      // Internal state (512 bits)
    uint8_t  keystream[64];  // Current keystream block
    size_t   position;       // Position in keystream
} salsa20_ctx;

/*----------------------------------------------
* Function Prototypes
------------------------------------------------*/

// Initialize Salsa20 context with key and nonce
void salsa20_init(salsa20_ctx* ctx, const uint8_t* key, const uint8_t* nonce);

// Set counter (for seeking in stream)
void salsa20_set_counter(salsa20_ctx* ctx, uint64_t counter);

// Encrypt/Decrypt data (same operation)
void salsa20_crypt(salsa20_ctx* ctx, uint8_t* data, size_t length);

// Generate keystream block
void salsa20_block(salsa20_ctx* ctx, uint8_t* output);
