#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include <math.h>
#include <bcrypt.h>

#pragma	comment (lib, "bcrypt.lib")

/*------------------------------------------------
 ## Report Error
--------------------------------------------------*/
BOOL ReportError(const char* Apiname) {

	fprintf(stderr, "\"%s\" Failed With Error : %d \n", Apiname, GetLastError());
	return FALSE;
}

/*------------------------------------------------
 ## Read Payload File
--------------------------------------------------*/
BOOL ReadPayloadFile(const char* InputFile, OUT PSIZE_T sPayloadSize, OUT PBYTE* pPayloadData) {

	HANDLE			hFile						= INVALID_HANDLE_VALUE;
	DWORD			dwFileSize					= 0;
	DWORD			dwNumberOfBytesRead			= 0;
	PBYTE			pBuffer						= NULL;

	// Opening the provided file
	hFile = CreateFileA(InputFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return ReportError("CreateFileA");
	}

	// Getting the file size
	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		CloseHandle(hFile);
		return ReportError("GetFileSize");
	}

	// Allocating buffer of size 'dwFileSize'
	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	if (!pBuffer) {
		CloseHandle(hFile);
		return ReportError("HeapAlloc");
	}

	// Reading the file
	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwNumberOfBytesRead, NULL)) {
		HeapFree(GetProcessHeap(), 0, pBuffer);
		CloseHandle(hFile);
		return ReportError("ReadFile");
	}

	*sPayloadSize = dwNumberOfBytesRead;
	*pPayloadData = pBuffer;

	return TRUE;
}

/*------------------------------------------------
 ## Write Payload File
--------------------------------------------------*/
BOOL WritePayloadFile(const char* InputFile, IN SIZE_T sPayloadSize, IN PBYTE pPayloadData) {

	HANDLE			hFile						= INVALID_HANDLE_VALUE;
	DWORD			dwNumberOfBytesWritten		= 0;

	// Opening the provided file
	hFile = CreateFileA(InputFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return ReportError("CreateFileA");
	}

	// Writing the file
	if (!WriteFile(hFile, pPayloadData, sPayloadSize, &dwNumberOfBytesWritten, NULL)) {
		CloseHandle(hFile);
		return ReportError("WriteFile");
	}

	return TRUE;
}

/*------------------------------------------------
 ## Generate Cryptographically Safe Random Bytes
--------------------------------------------------*/
BOOL GenerateRandomBytes(IN PBYTE pByte, IN SIZE_T sSize) {

	NTSTATUS			STATUS			= 0;

	STATUS = BCryptGenRandom(
		NULL,
		pByte,
		sSize,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);

	if (STATUS != 0x0) {
		return ReportError("BCryptGenRandom");
	}

	return TRUE;
}

/*------------------------------------------------
 ## Print Generated Hex Data As C-Array
--------------------------------------------------*/
VOID PrintHexData(IN LPCSTR lpName, IN PBYTE pData, IN SIZE_T sSize) {

	printf("unsigned char %s[] = {", lpName);

	for (int i = 0; i < sSize; i++) {
		if (i % 16 == 0)
			printf("\n\t");

		if (i < sSize - 1) {
			printf("0x%0.2X, ", pData[i]);
		}
		else {
			printf("0x%0.2X ", pData[i]);
		}
	}
	printf("\n};\n\n\n");
}

/*-------------------------------------------------
 ## Custom Padding Function
--------------------------------------------------*/
BOOL PadBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize) {

	PBYTE			PaddedBuffer			= NULL;
	SIZE_T			PaddedSize				= 0;

	PaddedSize   = InputBufferSize + 16 - (InputBufferSize % 16);
	PaddedBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, PaddedSize);
	if (!PaddedBuffer) return ReportError("HeapAlloc");

	RtlSecureZeroMemory(PaddedBuffer, PaddedSize);

	memcpy(PaddedBuffer, InputBuffer, InputBufferSize);

	*OutputPaddedBuffer  = PaddedBuffer;
	*OutputPaddedSize	 = PaddedSize;

	return TRUE;
}

/*-------------------------------------------------
 ## Print Banner
--------------------------------------------------*/
VOID PrintBanner() {
	printf("\n");
	printf("   _____ _               _      _____                  _   \n");
	printf("  / ____| |             | |    / ____|                | |  \n");
	printf(" | (___ | |__   __ _  __| | ___| |     _ __ _   _ _ __ | |_ \n");
	printf("  \\___ \\| '_ \\ / _` |/ _` |/ _ \\ |    | '__| | | | '_ \\| __|\n");
	printf("  ____) | | | | (_| | (_| |  __/ |____| |  | |_| | |_) | |_ \n");
	printf(" |_____/|_| |_|\\__,_|\\__,_|\\___|\\_____|_|   \\__, | .__/ \\__|\n");
	printf("                                             __/ | |        \n");
	printf("                                            |___/|_|        \n");
	printf("\n");
	printf("  Modern Encryption | Encoding | Obfuscation Tool\n");
	printf("  Author: Xec0xCC\n\n");
}


/*-------------------------------------------------
 ## Print Usage
--------------------------------------------------*/
VOID PrintUsage() {
	printf("\n");
	printf("USAGE:\n");
	printf("  ShadeCrypt.exe --file <path> [operations...]\n\n");

	printf("MODES:\n");
	printf("  --read          Console output (default)\n");
	printf("  --write         File output (needs --output)\n\n");

	printf("OPERATIONS:\n");
	printf("  --encrypt <algo>        xor, aes, chacha20, salsa20\n");
	printf("  --encode <method>       baseN\n");
	printf("  --obfuscate <type>      uuid, ipv6\n\n");

	printf("TEMPLATES (No --file needed):\n");
	printf("  --decrypt <algo>        Generate decrypt code\n");
	printf("  --decode <method>       Generate decode code\n");
	printf("  --deobfuscate <type>    Generate deobfuscate code\n\n");

	printf("EXAMPLES:\n");
	printf("  Basic:\n");
	printf("    ShadeCrypt.exe --file payload.bin --encrypt xor\n\n");

	printf("  Pipeline:\n");
	printf("    ShadeCrypt.exe --file payload.bin --encrypt aes --encode baseN --obfuscate uuid\n\n");

	printf("  Save to file:\n");
	printf("    ShadeCrypt.exe --write --file payload.bin --encrypt xor --output out.bin\n\n");

	printf("  Templates:\n");
	printf("    ShadeCrypt.exe --decrypt aes\n");
	printf("    ShadeCrypt.exe --deobfuscate uuid\n\n");
}

/*-------------------------------------------------
 ## Calculate Entropy
--------------------------------------------------*/
double CalculateEntropy(IN PBYTE pPayload, IN SIZE_T sPayloadSize) {

	double  entropy = 0.0;
	double  p = 0.0;
	long    counts[256] = { 0 };

	for (SIZE_T i = 0; i < sPayloadSize; i++) {
		counts[pPayload[i]]++;
	}

	for (int i = 0; i < 256; i++) {
		if (counts[i] > 0) {
			p = (double)counts[i] / sPayloadSize;
			entropy -= p * (log(p) / log(2.0));
		}
	}

	return entropy;
}