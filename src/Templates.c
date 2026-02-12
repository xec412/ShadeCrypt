/*--------------------------------------------------------------------
  UUID Deobfuscation Template
--------------------------------------------------------------------*/
const char* UUID_TEMPLATE =
"typedef RPC_STATUS(WINAPI* fnUuidFromStringA)(\n"
"	RPC_CSTR	StringUuid,\n"
"	UUID*		Uuid\n"
");\n\n"
"BOOL UuidDeobfuscation(IN CHAR* UuidArray[], IN SIZE_T NumberOfElements, OUT PBYTE* ppAddress, OUT SIZE_T* pDSize) {\n\n"
"	PBYTE								Buffer						= NULL,\n"
"										TmpBuffer					= NULL;\n\n"
"	SIZE_T								BufferSize					= 0;\n\n"
"	RPC_STATUS							STATUS						= 0;\n\n"
"	\n"
"	fnUuidFromStringA pUuidFromStringA = (fnUuidFromStringA)GetProcAddress(LoadLibrary(TEXT(\"RPCRT4\")), \"UuidFromStringA\");\n"
"	if (pUuidFromStringA == NULL) {\n"
"		printf(\"[!] GetProcAddress Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n\n"
"	BufferSize = NumberOfElements * 16;\n\n"
"	Buffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BufferSize);\n"
"	if (Buffer == NULL) {\n"
"		printf(\"[!] HeapAlloc Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n"
"	\n"
"	TmpBuffer = Buffer;\n\n"
"	for (int i = 0; i < NumberOfElements; i++) {\n\n"
"		if ((STATUS = pUuidFromStringA((RPC_CSTR)UuidArray[i], (UUID*)TmpBuffer)) != RPC_S_OK) {\n"
"			printf(\"[!] UuidFromStringA Failed With Error : 0x%0.8X \\n\", STATUS);\n"
"			return FALSE;\n"
"		}\n\n"
"		TmpBuffer = (PBYTE)(TmpBuffer + 16);\n"
"	}\n\n"
"	*ppAddress = Buffer;\n"
"	*pDSize	   = BufferSize;\n\n"
"	return TRUE;\n"
"}\n";


/*--------------------------------------------------------------------
  IPv6 Deobfuscation Template
--------------------------------------------------------------------*/
const char* IPV6_TEMPLATE =
"typedef NTSTATUS(NTAPI* fnRtlIpv6StringToAddressA)(\n"
"	PCSTR		S,\n"
"	PCSTR* Terminator,\n"
"	PVOID		Addr\n"
");\n\n"
"BOOL Ipv6Deobfuscation(IN CHAR* Ipv6Array[], IN SIZE_T NmbrOfElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize) {\n\n"
"	PBYTE		pBuffer = NULL,\n"
"		TmpBuffer = NULL;\n\n"
"	SIZE_T		sBuffSize = NULL;\n\n"
"	PCSTR		Terminator = NULL;\n\n"
"	NTSTATUS	STATUS = NULL;\n\n"
"	fnRtlIpv6StringToAddressA pRtlIpv6StringToAddressA = (fnRtlIpv6StringToAddressA)GetProcAddress(GetModuleHandle(TEXT(\"NTDLL\")), \"RtlIpv6StringToAddressA\");\n"
"	if (pRtlIpv6StringToAddressA == NULL) {\n"
"		printf(\"[!] GetProcAddress Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n\n"
"	sBuffSize = NmbrOfElements * 16;\n\n"
"	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, sBuffSize);\n"
"	if (pBuffer == NULL) {\n"
"		printf(\"[!] HeapAlloc Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n\n"
"	TmpBuffer = pBuffer;\n\n"
"	for (int i = 0; i < NmbrOfElements; i++) {\n\n"
"		if ((STATUS = pRtlIpv6StringToAddressA(Ipv6Array[i], &Terminator, TmpBuffer)) != 0x0) {\n"
"			printf(\"[!] RtlIpv6StringToAddressA Failed At [%s] With Error 0x%0.8X\", Ipv6Array[i], STATUS);\n"
"			return FALSE;\n"
"		}\n\n"
"		TmpBuffer = (PBYTE)(TmpBuffer + 16);\n\n"
"	}\n\n"
"	*ppDAddress = pBuffer;\n"
"	*pDSize = sBuffSize;\n\n"
"	return TRUE;\n"
"}\n";


/*--------------------------------------------------------------------
  MAC Deobfuscation Template
  (Uses ntdll!RtlEthernetStringToAddressA)
--------------------------------------------------------------------*/
const char* MAC_TEMPLATE =
"typedef NTSTATUS(NTAPI* fnRtlEthernetStringToAddressA)(\n"
"	PCSTR		S,\n"
"	PCSTR* Terminator,\n"
"	PVOID		Addr\n"
");\n\n"
"BOOL MacDeobfuscation(IN CHAR* MacArray[], IN SIZE_T NmbrOfElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize) {\n\n"
"	PBYTE		pBuffer = NULL,\n"
"		TmpBuffer = NULL;\n\n"
"	SIZE_T		sBuffSize = NULL;\n\n"
"	PCSTR		Terminator = NULL;\n\n"
"	NTSTATUS	STATUS = NULL;\n\n"
"	fnRtlEthernetStringToAddressA pRtlEthernetStringToAddressA = (fnRtlEthernetStringToAddressA)GetProcAddress(GetModuleHandle(TEXT(\"NTDLL\")), \"RtlEthernetStringToAddressA\");\n"
"	if (pRtlEthernetStringToAddressA == NULL) {\n"
"		printf(\"[!] GetProcAddress Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n\n"
"	sBuffSize = NmbrOfElements * 6;\n\n"
"	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), 0, sBuffSize);\n"
"	if (pBuffer == NULL) {\n"
"		printf(\"[!] HeapAlloc Failed With Error : %d \\n\", GetLastError());\n"
"		return FALSE;\n"
"	}\n\n"
"	TmpBuffer = pBuffer;\n\n"
"	for (int i = 0; i < NmbrOfElements; i++) {\n\n"
"		if ((STATUS = pRtlEthernetStringToAddressA(MacArray[i], &Terminator, TmpBuffer)) != 0x0) {\n"
"			printf(\"[!] RtlEthernetStringToAddressA Failed At [%s] With Error 0x%0.8X\", MacArray[i], STATUS);\n"
"			return FALSE;\n"
"		}\n\n"
"		TmpBuffer = (PBYTE)(TmpBuffer + 6);\n\n"
"	}\n\n"
"	*ppDAddress = pBuffer;\n"
"	*pDSize = sBuffSize;\n\n"
"	return TRUE;\n"
"}\n";


/*--------------------------------------------------------------------
  AES Decryption Template
  (Using Tiny-AES library structure)
--------------------------------------------------------------------*/
const char* AES_TEMPLATE =
"BOOL Aes_Decrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE iv) {\n\n"
"	struct AES_ctx ctx;\n\n"
"	AES_init_ctx_iv(&ctx, key, iv);\n"
"	AES_CBC_decrypt_buffer(&ctx, data, size);\n\n"
"	return true;\n"
"}\n";


/*--------------------------------------------------------------------
  ChaCha20 Decryption Template
  (Uses the same logic as encryption)
--------------------------------------------------------------------*/
const char* CHACHA_TEMPLATE =
"BOOL ChaCha20_Decrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE iv) {\n\n"
"	if (!data || !key || !iv || size == 0) {\n"
"		return false;\n"
"	}\n\n"
"	return chacha_memory(\n"
"		key, KEY_LENGTH,\n"
"		20,\n"
"		iv,  CHACHA_IV_LENGTH,\n"
"		1,\n"
"		data,\n"
"		(unsigned long)size,\n"
"		data\n"
"	) == CRYPT_OK;\n\n"
"}\n";


/*--------------------------------------------------------------------
  XOR Decryption Template
  (Simple repeated key XOR logic)
--------------------------------------------------------------------*/
const char* XOR_TEMPLATE =
"VOID XorDecrypt(PBYTE data, SIZE_T size, PBYTE key, SIZE_T keySize) {\n\n"
"	for (size_t i = 0, j = 0; i < size; i++, j++) {\n"
"		if (j >= keySize) {\n"
"			j = 0;\n"
"		}\n"
"		data[i] = data[i] ^ key[j];\n"
"	}\n"
"}\n";

/*--------------------------------------------------------------------
  Salsa Decryption Template
  (Uses the same logic as encryption)
--------------------------------------------------------------------*/
const char* SALSA_TEMPLATE =
"BOOL Salsa20_Decrypt(PBYTE data, SIZE_T size, PBYTE key, PBYTE nonce) {\n\n"
"	if (!data || !key || !nonce || size == 0) {\n"
"		return FALSE;\n"
"	}\n\n"
"	salsa20_ctx ctx;\n"
"	salsa20_init(&ctx, key, nonce);\n"
"	salsa20_crypt(&ctx, data, size);\n\n"
"	return TRUE;\n"
"}\n";