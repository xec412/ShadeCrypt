#include "Common.h"
#include <stdio.h>

/*----------------------------------------------
* ChaCha20 Encrypt Function
------------------------------------------------*/
BOOL ChaCha20Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Iv) {

	if (!Data || Size == 0) {
		printf("[!] Invalid Inputs \n");
		return FALSE;
	}

	return chacha_memory(
		Key, KeyLength,
		20,
		Iv,  ChaChaIvLength,
		1,
		Data,
		(unsigned long)Size,
		Data
	) == CRYPT_OK;
}