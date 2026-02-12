#include "Common.h"

/*----------------------------------------------
* Read binary file function
------------------------------------------------*/
BOOL ReadBinaryFile(IN LPCSTR lpFileName, OUT PBYTE* ppBuffer, OUT PDWORD pdwFileSize) {

	HANDLE							hFile						= INVALID_HANDLE_VALUE;

	DWORD							dwFileSize					= 0;
	
	DWORD							dwNumberOfBytesRead			= 0;

	PBYTE							pBuffer						= NULL;

	// Opening the file which provided by the user
	hFile = CreateFileA(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("[!] CreateFileA Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	// Getting File Size
	dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		printf("[!] GetFileSize Failed With Error : %d \n", GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	// Allocating buffer
	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	if (pBuffer == NULL) {
		printf("[!] HeapAlloc Failed With Error : %d \n", GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	// Reading provided file
	if (!ReadFile(hFile, pBuffer, dwFileSize, &dwNumberOfBytesRead, NULL)) {
		printf("[!] ReadFile Failed With Error : %d \n", GetLastError());
		CloseHandle(hFile);
		return FALSE;
	}

	*ppBuffer		= pBuffer;
	*pdwFileSize	= dwFileSize;

	CloseHandle(hFile);
	return TRUE;
}

/*----------------------------------------------
* Custom Padding Function
------------------------------------------------*/
BOOL PadBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize) {
	
	SIZE_T											PaddedSize				= 0;
	PBYTE											PaddedBuffer			= NULL;
	
	PaddedSize   = InputBufferSize + 16 - (InputBufferSize % 16);
	PaddedBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, PaddedSize);
	if (!PaddedBuffer) return FALSE;

	RtlSecureZeroMemory(PaddedBuffer, PaddedSize);

	memcpy(PaddedBuffer, InputBuffer, InputBufferSize);

	*OutputPaddedBuffer = PaddedBuffer;
	*OutputPaddedSize	= PaddedSize;

	return TRUE;
}

/*----------------------------------------------
* Custom Padding Function For Mac Obfuscation
------------------------------------------------*/
BOOL PadMacBuffer(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* OutputPaddedBuffer, OUT SIZE_T* OutputPaddedSize) {

	SIZE_T									PaddedSize					= 0;
	PBYTE									PaddedBuffer				= NULL;

	PaddedSize   = InputBufferSize + 6 - (InputBufferSize % 6);
	PaddedBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, PaddedSize);
	if (!PaddedBuffer) return FALSE;

	RtlSecureZeroMemory(PaddedBuffer, PaddedSize);

	memcpy(PaddedBuffer, InputBuffer, InputBufferSize);

	*OutputPaddedBuffer = PaddedBuffer;
	*OutputPaddedSize	= PaddedSize;

	return TRUE;
}

/*----------------------------------------------
* Print Hex Data Function
------------------------------------------------*/
VOID PrintHexData(IN LPCSTR Name, IN PBYTE Data, IN SIZE_T Size) {

	printf("unsigned char %s[] = {", Name);

	for (int i = 0; i < Size; i++) {
		
		if (i % 16 == 0)
			printf("\n\t");

		if (i < Size - 1) {
			printf("0x%0.2X, ", Data[i]);
		}
		else {
			printf("0x%0.2X ", Data[i]);
		}
	}
	printf("\n};\n\n\n");
}

/*----------------------------------------------
* Generate Random Bytes Function
------------------------------------------------*/
BOOL GenerateRandomBytes(IN PBYTE Byte, IN SIZE_T Size) {

	NTSTATUS					STATUS					= 0;

	if (!Byte || Size == 0) {
		return FALSE;
	}

	STATUS = BCryptGenRandom(
		NULL,
		Byte,
		Size,
		BCRYPT_USE_SYSTEM_PREFERRED_RNG
	);

	if (STATUS != STATUS_SUCCESS) {
		printf("BCryptGenRandom Failed With Error : 0x%0.8X \n", STATUS);
		return FALSE;
	}

	return TRUE;
}