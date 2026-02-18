#include <Windows.h>
#include <stdio.h>
#include "Common.h"

/*------------------------------------------------
 ## Xor Encryption
--------------------------------------------------*/
VOID XorEncrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN SIZE_T sKeySize) {

	if (!pPayload || !pKey || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return;
	}

	for (size_t i = 0, j = 0; i < sPayloadSize; i++, j++) {

		if (j >= sKeySize) {
			j = 0;
		}
		pPayload[i] = pPayload[i] ^ pKey[j];
	}
}