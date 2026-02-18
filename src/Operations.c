// Operations.c
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Operations.h"
#include "Common.h"

/*-------------------------------------------------
 ## Execute All Operations
--------------------------------------------------*/
BOOL ExecuteOperations(IN PARGS pArgs, IN OUT PBYTE* ppPayload, IN OUT PSIZE_T psPayloadSize) {

	if (!pArgs || !ppPayload || !psPayloadSize) {
		return FALSE;
	}

	PBYTE	pPayload = *ppPayload;
	SIZE_T	sPayloadSize = *psPayloadSize;

	// === ENCRYPTION ===
	if (pArgs->EncryptionType) {
		printf("[*] Encryption: %s\n", pArgs->EncryptionType);

		BYTE bKey[KeyLength] = { 0 };
		BYTE bIv[IvLength] = { 0 };
		BYTE bNonce[Salsa20NonceLength] = { 0 };

		if (strcmp(pArgs->EncryptionType, "xor") == 0) {
			if (!GenerateRandomBytes(bKey, KeyLength)) return FALSE;
			XorEncrypt(pPayload, sPayloadSize, bKey, KeyLength);
			printf("[+] XOR encryption complete\n\n");
			PrintHexData("XorKey", bKey, KeyLength);
		}
		else if (strcmp(pArgs->EncryptionType, "aes") == 0) {
			if (!GenerateRandomBytes(bKey, KeyLength) || !GenerateRandomBytes(bIv, IvLength)) return FALSE;
			if (!AesEncrypt(pPayload, sPayloadSize, bKey, bIv)) return FALSE;
			printf("[+] AES encryption complete\n\n");
			PrintHexData("AesKey", bKey, KeyLength);
			PrintHexData("AesIv", bIv, IvLength);
		}
		else if (strcmp(pArgs->EncryptionType, "chacha20") == 0) {
			if (!GenerateRandomBytes(bKey, KeyLength) || !GenerateRandomBytes(bIv, ChaChaIvLength)) return FALSE;
			if (!ChaCha20Encrypt(pPayload, sPayloadSize, bKey, bIv)) return FALSE;
			printf("[+] ChaCha20 encryption complete\n\n");
			PrintHexData("ChaCha20Key", bKey, KeyLength);
			PrintHexData("ChaCha20Iv", bIv, ChaChaIvLength);
		}
		else if (strcmp(pArgs->EncryptionType, "salsa20") == 0) {
			if (!GenerateRandomBytes(bKey, KeyLength) || !GenerateRandomBytes(bNonce, Salsa20NonceLength)) return FALSE;
			if (!Salsa20Encrypt(pPayload, sPayloadSize, bKey, bNonce)) return FALSE;
			printf("[+] Salsa20 encryption complete\n\n");
			PrintHexData("Salsa20Key", bKey, KeyLength);
			PrintHexData("Salsa20Nonce", bNonce, Salsa20NonceLength);
		}
		else {
			fprintf(stderr, "[-] Unknown encryption type: %s\n", pArgs->EncryptionType);
			return FALSE;
		}
	}

	// === ENCODING ===
	if (pArgs->EncodingType) {
		printf("[*] Encoding: %s\n", pArgs->EncodingType);

		if (strcmp(pArgs->EncodingType, "baseN") == 0) {
			PBYTE	pEncodedBuffer = NULL;
			SIZE_T	sEncodedSize = 0;

			BaseNEncode(pPayload, sPayloadSize, &pEncodedBuffer, &sEncodedSize, TRUE);
			if (!pEncodedBuffer) return FALSE;

			HeapFree(GetProcessHeap(), 0, pPayload);
			pPayload = pEncodedBuffer;
			sPayloadSize = sEncodedSize;

			printf("[+] BaseN encoding complete\n");
			printf("[+] Encoded size: %zu bytes\n\n", sPayloadSize);
		}
		else {
			fprintf(stderr, "[-] Unknown encoding type: %s\n", pArgs->EncodingType);
			return FALSE;
		}
	}

	// === OBFUSCATION ===
	if (pArgs->ObfuscationType) {
		printf("[*] Obfuscation: %s\n", pArgs->ObfuscationType);

		if (strcmp(pArgs->ObfuscationType, "uuid") == 0) {
			if (!GenerateUuidOutput(pPayload, sPayloadSize)) return FALSE;
			printf("\n[+] UUID obfuscation complete\n");

			// Don't update payload (text output already printed)
			return TRUE;
		}
		else if (strcmp(pArgs->ObfuscationType, "ipv6") == 0) {
			if (!GenerateIpv6Output(pPayload, sPayloadSize)) return FALSE;
			printf("\n[+] IPv6 obfuscation complete\n");

			// Don't update payload (text output already printed)
			return TRUE;
		}
		else {
			fprintf(stderr, "[-] Unknown obfuscation type: %s\n", pArgs->ObfuscationType);
			return FALSE;
		}
	}

	// Update output
	*ppPayload = pPayload;
	*psPayloadSize = sPayloadSize;

	return TRUE;
}