#pragma once

#include <Windows.h>

typedef struct _ARGS {
	
	// Modes
	BOOL	bReadMode;
	BOOL	bWriteMode;

	// File Paths
	char*	InputFilePath;
	char*   OutputFileName;

	// Operations
	char*	EncryptionType;
	char*	EncodingType;
	char*	ObfuscationType;
	char*	DecryptionType;
	char*	DecodingType;
	char*	DeobfuscationType;
} ARGS, *PARGS;

// Parse command line arguments
BOOL ParseArguments(int argc, char* argv[], OUT PARGS pArgs);

// Validate parsed arguments
BOOL ValidateArguments(IN PARGS pArgs);