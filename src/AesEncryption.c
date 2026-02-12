#include "Common.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------
* Aes Encrypt Function
------------------------------------------------*/
BOOL Aes_Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Iv) {

	PBYTE						PaddedData					= NULL;
	SIZE_T						PaddedSize					= 0;

	if (!Data || Size == 0) {
		printf("[!] Invalid Inputs \n");
		return FALSE;
	}

	// [!] In case file size is not a multiple of 16.
	if (Size % 16 != 0) {
		PadBuffer(Data, Size, &PaddedData, &PaddedSize);
		Data = PaddedData;
		Size = PaddedSize;
	}

	struct AES_ctx ctx;

	AES_init_ctx_iv(&ctx, Key, Iv);

	AES_CBC_encrypt_buffer(&ctx, Data, Size);

	return TRUE;
}