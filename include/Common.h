#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <ntstatus.h>
#include <bcrypt.h>
#include "Templates.h"
#include "aes.h"
#include "ChaCha.h"
#include "Salsa20.h"

#pragma comment (lib, "bcrypt.lib")

/*----------------------------------------------
* Encryption Algorithm Constants
------------------------------------------------*/
#define		KeyLength			32
#define		IvLength			16
#define		ChaChaIvLength		12  // ChaCha20 Algorithm uses 12 byte iv
#define     SalsaNonceLength	8

/*----------------------------------------------
* Function Prototypes
------------------------------------------------*/

// From Utils.c
BOOL ReadBinaryFile(IN LPCSTR lpFileName, OUT PBYTE* ppBuffer, OUT PDWORD pdwFileSize);
BOOL PadBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize);
VOID PrintHexData(IN LPCSTR Name, IN PBYTE Data, IN SIZE_T Size);
BOOL GenerateRandomBytes(IN PBYTE Byte, IN SIZE_T Size);
BOOL PadMacBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize);

// From UuidObfuscation.c
BOOL GenerateUuidOutput(unsigned char* pData, SIZE_T sSize);

// From Ipv6Obfuscation.c
BOOL GenerateIpv6Output(unsigned char* pData, SIZE_T sSize);

// From MacObfuscation.c
BOOL GenerateMacOutput(unsigned char* pData, SIZE_T sSize);

// From XorEncryption.c
VOID XorEncrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN SIZE_T KeySize);

// From AesEncryption.c
BOOL Aes_Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Iv);

// From ChaChaEncryption.c
BOOL ChaCha20Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Iv);

// From Salsa20.c
BOOL Salsa20Encrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN PBYTE Nonce);