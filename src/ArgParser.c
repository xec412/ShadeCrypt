#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include "ArgParser.h"

/*-------------------------------------------------
 ## Parsing Arguments
--------------------------------------------------*/
BOOL ParseArguments(int argc, char* argv[], OUT PARGS pArgs) {

	if (!pArgs) {
		return FALSE;
	}

	// Initializing with defaults
	RtlSecureZeroMemory(pArgs, sizeof(ARGS));
	pArgs->bReadMode  = TRUE; // Default: READ mode
	pArgs->bWriteMode = FALSE;

	// Parsing Arguments
	for (int i = 1; i < argc; i++) {

		if (_stricmp(argv[i], "--help") == 0) {
			PrintUsage();
			return 0;
		}
		else if (_stricmp(argv[i], "--read") == 0) {
			pArgs->bReadMode  = TRUE;
			pArgs->bWriteMode = FALSE;
		}
		else if (_stricmp(argv[i], "--write") == 0) {
			pArgs->bWriteMode = TRUE;
			pArgs->bReadMode  = FALSE;	
		}
		else if (_stricmp(argv[i], "--file") == 0 && (i + 1) < argc) {
			pArgs->InputFilePath = argv[++i];
		}
		else if (_stricmp(argv[i], "--output") == 0 && (i + 1) < argc) {
			pArgs->OutputFileName = argv[++i];
		}
		else if (_stricmp(argv[i], "--encrypt") == 0 && (i + 1) < argc) {
			pArgs->EncryptionType = argv[++i];
		}
		else if (_stricmp(argv[i], "--encode") == 0 && (i + 1) < argc) {
			pArgs->EncodingType = argv[++i];
		}
		else if (_stricmp(argv[i], "--obfuscate") == 0 && (i + 1) < argc) {
			pArgs->ObfuscationType = argv[++i];
		}
		else if (_stricmp(argv[i], "--decrypt") == 0 && (i + 1) < argc) {
			pArgs->DecryptionType  = argv[++i];
		}
		else if (_stricmp(argv[i], "--decode") == 0 && (i + 1) < argc) {
			pArgs->DecodingType    = argv[++i];
		}
		else if (_stricmp(argv[i], "--deobfuscate") == 0 && (i + 1) < argc) {
			pArgs->DeobfuscationType = argv[++i];
		}
		else {
			fprintf(stderr, "[!] Invalid Argument: %s \n", argv[i]);
			return FALSE;
		}
	}

	return TRUE;
}

/*-------------------------------------------------
 ## Validate Parsed Arguments
--------------------------------------------------*/
BOOL ValidateArguments(IN PARGS pArgs) {

	if (!pArgs) {
		return FALSE;
	}

	// Template mode - no file needed
	if (pArgs->DecryptionType || pArgs->DecodingType || pArgs->DeobfuscationType) {
		return TRUE;
	}

	// Normal mode validation
	if (!pArgs->InputFilePath) {
		fprintf(stderr, "[!] Error : Missing --file Argument \n");
		PrintUsage();
		return FALSE;
	}

	if (!pArgs->EncryptionType && !pArgs->EncodingType && !pArgs->ObfuscationType) {
		fprintf(stderr, "[!] No Operations Specified \n");
		fprintf(stderr, "[i] Use At Least One: --encrypt, --encode, or --obfuscate");
		PrintUsage();
		return FALSE;
	}

	if (pArgs->bWriteMode && !pArgs->OutputFileName) {
		fprintf(stderr, "[!] Error : --write Mode Requires --output Argument \n");
		PrintUsage();
		return FALSE;
	}

	return TRUE;
}