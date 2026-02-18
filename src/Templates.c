// Templates.c
#include <Windows.h>
#include <stdio.h>
/*-------------------------------------------------
 ## Generate XOR Decryption Template
--------------------------------------------------*/
VOID GenerateXorDecryptTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("    XOR DECRYPTION TEMPLATE\n");
	printf("========================================\n\n");

	printf("VOID XorDecrypt(PBYTE data, SIZE_T size, PBYTE key, SIZE_T keySize) {\n\n");
	printf("	for (size_t i = 0, j = 0; i < size; i++, j++) {\n");
	printf("		if (j >= keySize) {\n");
	printf("			j = 0;\n");
	printf("		}\n");
	printf("		data[i] = data[i] ^ key[j];\n");
	printf("	}\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate AES Decryption Template
--------------------------------------------------*/
VOID GenerateAesDecryptTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("    AES DECRYPTION TEMPLATE\n");
	printf("========================================\n\n");

	printf("BOOL AesDecrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE iv) {\n\n");
	printf("	struct AES_ctx ctx;\n\n");
	printf("	AES_init_ctx_iv(&ctx, key, iv);\n");
	printf("	AES_CBC_decrypt_buffer(&ctx, data, size);\n\n");
	printf("	return TRUE;\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate ChaCha20 Decryption Template
--------------------------------------------------*/
VOID GenerateChaCha20DecryptTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("   CHACHA20 DECRYPTION TEMPLATE\n");
	printf("========================================\n\n");

	printf("BOOL ChaCha20Decrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE iv) {\n\n");
	printf("	if (!data || !key || !iv || size == 0) {\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");
	printf("	return chacha_memory(\n");
	printf("		key, 32,\n");
	printf("		20,\n");
	printf("		iv, 12,\n");
	printf("		1,\n");
	printf("		data,\n");
	printf("		(unsigned long)size,\n");
	printf("		data\n");
	printf("	) == CRYPT_OK;\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate Salsa20 Decryption Template
--------------------------------------------------*/
VOID GenerateSalsa20DecryptTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("   SALSA20 DECRYPTION TEMPLATE\n");
	printf("========================================\n\n");

	printf("BOOL Salsa20Decrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE nonce) {\n\n");
	printf("	if (!data || !key || !nonce || size == 0) {\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");
	printf("	salsa20_ctx ctx;\n");
	printf("	salsa20_init(&ctx, key, nonce);\n");
	printf("	salsa20_crypt(&ctx, data, size);\n\n");
	printf("	return TRUE;\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate BaseN Decoding Template
--------------------------------------------------*/
VOID GenerateBaseNDecodeTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("      BASEN DECODING TEMPLATE\n");
	printf("========================================\n\n");

	printf("#define BASE_N 5\n\n");

	printf("VOID BaseNDecode(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* ppOutputBuffer, OUT PSIZE_T psOutputSize) {\n\n");

	printf("	if (!InputBuffer || !InputBufferSize || !ppOutputBuffer) return;\n\n");

	printf("	SIZE_T TmpSize = *psOutputSize = (BASE_N * InputBufferSize - (BASE_N * 8 - 4)) / 8;\n");
	printf("	*ppOutputBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, TmpSize);\n");
	printf("	if (!*ppOutputBuffer) return;\n\n");

	printf("	unsigned char* TmpOutputBuffer = (unsigned char*)*ppOutputBuffer;\n");
	printf("	unsigned char* TmpInputBuffer = (unsigned char*)InputBuffer;\n");
	printf("	unsigned int X = 0, Z = 0;\n\n");

	printf("	// Writing 8 bits, Reading BASE_N bits\n");
	printf("	BYTE WL = 8, RL = BASE_N, MV = (1 << 8) - 1;\n\n");

	printf("	while (TmpSize) {\n");
	printf("		X = (X << RL) | (*TmpInputBuffer++ & ((1 << BASE_N) - 1));\n");
	printf("		Z += RL;\n");
	printf("		while (Z >= WL) {\n");
	printf("			Z -= WL;\n");
	printf("			*TmpOutputBuffer++ = (X >> Z) & MV;\n");
	printf("			TmpSize--;\n");
	printf("			if (TmpSize == 0) break;\n");
	printf("		}\n");
	printf("	}\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate UUID Deobfuscation Template
--------------------------------------------------*/
VOID GenerateUuidDeobfuscateTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("   UUID DEOBFUSCATION TEMPLATE\n");
	printf("========================================\n\n");

	printf("typedef RPC_STATUS(WINAPI* fnUuidFromStringA)(\n");
	printf("	RPC_CSTR	StringUuid,\n");
	printf("	UUID*		Uuid\n");
	printf(");\n\n");

	printf("BOOL UuidDeobfuscation(IN CHAR* UuidArray[], IN SIZE_T NumberOfElements, OUT PBYTE* ppAddress, OUT SIZE_T* pDSize) {\n\n");
	printf("	PBYTE			Buffer = NULL,\n");
	printf("				TmpBuffer = NULL;\n");
	printf("	SIZE_T			BufferSize = 0;\n");
	printf("	RPC_STATUS		STATUS = 0;\n\n");

	printf("	fnUuidFromStringA pUuidFromStringA = (fnUuidFromStringA)GetProcAddress(LoadLibrary(TEXT(\"RPCRT4\")), \"UuidFromStringA\");\n");
	printf("	if (pUuidFromStringA == NULL) {\n");
	printf("		printf(\"[!] GetProcAddress Failed With Error : %%d \\n\", GetLastError());\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");

	printf("	BufferSize = NumberOfElements * 16;\n\n");

	printf("	Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BufferSize);\n");
	printf("	if (Buffer == NULL) {\n");
	printf("		printf(\"[!] HeapAlloc Failed With Error : %%d \\n\", GetLastError());\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");

	printf("	TmpBuffer = Buffer;\n\n");

	printf("	for (int i = 0; i < NumberOfElements; i++) {\n");
	printf("		if ((STATUS = pUuidFromStringA((RPC_CSTR)UuidArray[i], (UUID*)TmpBuffer)) != RPC_S_OK) {\n");
	printf("			printf(\"[!] UuidFromStringA Failed With Error : 0x%%0.8X \\n\", STATUS);\n");
	printf("			return FALSE;\n");
	printf("		}\n");
	printf("		TmpBuffer = (PBYTE)(TmpBuffer + 16);\n");
	printf("	}\n\n");

	printf("	*ppAddress = Buffer;\n");
	printf("	*pDSize = BufferSize;\n\n");

	printf("	return TRUE;\n");
	printf("}\n\n");
}

/*-------------------------------------------------
 ## Generate IPv6 Deobfuscation Template
--------------------------------------------------*/
VOID GenerateIpv6DeobfuscateTemplate() {
	printf("\n");
	printf("========================================\n");
	printf("   IPV6 DEOBFUSCATION TEMPLATE\n");
	printf("========================================\n\n");

	printf("typedef NTSTATUS(NTAPI* fnRtlIpv6StringToAddressA)(\n");
	printf("	PCSTR		S,\n");
	printf("	PCSTR*		Terminator,\n");
	printf("	PVOID		Addr\n");
	printf(");\n\n");

	printf("BOOL Ipv6Deobfuscation(IN CHAR* Ipv6Array[], IN SIZE_T NmbrOfElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize) {\n\n");
	printf("	PBYTE		pBuffer = NULL,\n");
	printf("			TmpBuffer = NULL;\n");
	printf("	SIZE_T		sBuffSize = 0;\n");
	printf("	PCSTR		Terminator = NULL;\n");
	printf("	NTSTATUS	STATUS = 0;\n\n");

	printf("	fnRtlIpv6StringToAddressA pRtlIpv6StringToAddressA = (fnRtlIpv6StringToAddressA)GetProcAddress(GetModuleHandle(TEXT(\"NTDLL\")), \"RtlIpv6StringToAddressA\");\n");
	printf("	if (pRtlIpv6StringToAddressA == NULL) {\n");
	printf("		printf(\"[!] GetProcAddress Failed With Error : %%d \\n\", GetLastError());\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");

	printf("	sBuffSize = NmbrOfElements * 16;\n\n");

	printf("	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, sBuffSize);\n");
	printf("	if (pBuffer == NULL) {\n");
	printf("		printf(\"[!] HeapAlloc Failed With Error : %%d \\n\", GetLastError());\n");
	printf("		return FALSE;\n");
	printf("	}\n\n");

	printf("	TmpBuffer = pBuffer;\n\n");

	printf("	for (int i = 0; i < NmbrOfElements; i++) {\n");
	printf("		if ((STATUS = pRtlIpv6StringToAddressA(Ipv6Array[i], &Terminator, TmpBuffer)) != 0x0) {\n");
	printf("			printf(\"[!] RtlIpv6StringToAddressA Failed At [%%s] With Error 0x%%0.8X\", Ipv6Array[i], STATUS);\n");
	printf("			return FALSE;\n");
	printf("		}\n");
	printf("		TmpBuffer = (PBYTE)(TmpBuffer + 16);\n");
	printf("	}\n\n");

	printf("	*ppDAddress = pBuffer;\n");
	printf("	*pDSize = sBuffSize;\n\n");

	printf("	return TRUE;\n");
	printf("}\n\n");
}