#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include "ChaCha.h"

/*------------------------------------------------
 ## ChaCha20 Encryption
--------------------------------------------------*/
BOOL ChaCha20Encrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pIv) {

	if (!pPayload || !pKey || !pIv || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return FALSE;
	}

	return chacha_memory(
		pKey, KeyLength,
		20,
		pIv,  ChaChaIvLength,
		1,
		pPayload,
		(unsigned long)sPayloadSize,
		pPayload
	) == CRYPT_OK;

}