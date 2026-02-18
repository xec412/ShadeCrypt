#pragma once

#include <Windows.h>
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN

/*------------------------------------------------
 ## Operation Method Constants
--------------------------------------------------*/
#define		KeyLength				32
#define		IvLength				16
#define		ChaChaIvLength			12 // ChaCha20 algorithm uses 12 byte IV
#define		Salsa20NonceLength		8
#define		BASE_N					5  // Base value for both encoding and decoding
#define     CALC_ENC_SIZE(number)  ((number + 4) / BASE_N * 8)
#define     ROUND_UP(number) ((number % 5 == 0) ? (number) : (((number) / 5) + 1) * 5)

/*------------------------------------------------
 ## Function Prototypes
--------------------------------------------------*/

// From Utils.c
BOOL ReportError(const char* Apiname);
BOOL ReadPayloadFile(const char* InputFile, OUT PSIZE_T sPayloadSize, OUT PBYTE* pPayloadData);
BOOL WritePayloadFile(const char* InputFile, IN SIZE_T sPayloadSize, IN PBYTE pPayloadData);
BOOL GenerateRandomBytes(IN PBYTE pByte, IN SIZE_T sSize);
VOID PrintHexData(IN LPCSTR lpName, IN PBYTE pData, IN SIZE_T sSize);
BOOL PadBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize);
VOID PrintBanner();
VOID PrintUsage();
double CalculateEntropy(IN PBYTE pPayload, IN SIZE_T sPayloadSize);

// From UuidObfuscation.c
BOOL GenerateUuidOutput(IN PBYTE pPayload, IN SIZE_T sPayloadSize);

// From Ipv6Obfuscation.c
BOOL GenerateIpv6Output(IN PBYTE pPayload, IN SIZE_T sPayloadSize);

// From BaseN-Encode.c
VOID BaseNEncode(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* ppOutputBuffer, OUT PSIZE_T psOutputSize, IN BOOL bEncode);

// From XorEncryption.c
VOID XorEncrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN SIZE_T sKeySize);

// From AesEncryption.c
BOOL AesEncrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pIv);

// From ChaChaEncryption.c
BOOL ChaCha20Encrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pIv);

// From SalsaEncryption.c
BOOL Salsa20Encrypt(IN PBYTE pPayload, IN SIZE_T sPayloadSize, IN PBYTE pKey, IN PBYTE pNonce);

// From Templates.c
VOID GenerateXorDecryptTemplate();
VOID GenerateAesDecryptTemplate();
VOID GenerateChaCha20DecryptTemplate();
VOID GenerateSalsa20DecryptTemplate();
VOID GenerateBaseNDecodeTemplate();
VOID GenerateUuidDeobfuscateTemplate();
VOID GenerateIpv6DeobfuscateTemplate();
