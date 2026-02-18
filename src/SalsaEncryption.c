#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include "Salsa20.h"

/*------------------------------------------------
 ## Salsa20 Encryption
--------------------------------------------------*/
BOOL Salsa20Encrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pNonce) {

	if (!pPayload || !pKey || !pNonce || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return FALSE;
	}

	salsa20_ctx ctx;

	salsa20_init(&ctx, pKey, pNonce);

	salsa20_crypt(&ctx, pPayload, sPayloadSize);

	return TRUE;
}