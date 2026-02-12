#include "Common.h"
#include <stdlib.h>
#include <stdio.h>

/*----------------------------------------------
* Salsa20 Encrypt Function
------------------------------------------------*/
BOOL Salsa20Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Nonce) {

	salsa20_ctx ctx;

	if (!Data || Size == 0) {
		printf("[!] Invalid Inputs \n");
		return FALSE;
	}

	salsa20_init(&ctx, Key, Nonce);
	salsa20_crypt(&ctx, Data, Size);

	return TRUE;
}