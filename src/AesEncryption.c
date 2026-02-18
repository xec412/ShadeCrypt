#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include "aes.h"

/*------------------------------------------------
 ## Aes Encryption
--------------------------------------------------*/
BOOL AesEncrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pIv) {

	PBYTE			PaddedBuffer			= NULL;
	SIZE_T			PaddedSize				= 0;

	if (!pPayload || !pKey || !pIv || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return FALSE;
	}

	// Padding the payload if it's not a multiple of 16
	if (sPayloadSize % 16 != 0) {
		if (!PadBuffer(pPayload, sPayloadSize, &PaddedBuffer, &PaddedSize)) {
			return FALSE;
		}
		pPayload		= PaddedBuffer;
		sPayloadSize	= PaddedSize;
	}

	struct AES_ctx ctx;

	AES_init_ctx_iv(&ctx, pKey, pIv);

	AES_CBC_encrypt_buffer(&ctx, pPayload, sPayloadSize);

	return TRUE;
}