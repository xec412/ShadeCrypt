// ShadeCrypt.c
#include <Windows.h>
#include <stdio.h>
#include "Common.h"
#include "ArgParser.h"
#include "Operations.h"

/*-------------------------------------------------
 ## Main Entry Point
--------------------------------------------------*/
int main(int argc, char* argv[]) {

	PrintBanner();

	if (argc < 2) {
		PrintUsage();
		return -1;
	}

	// === PARSE ARGUMENTS ===
	ARGS args = { 0 };

	if (!ParseArguments(argc, argv, &args)) {
		return -1;
	}

	// === TEMPLATE MODE ===
	if (args.DecryptionType || args.DecodingType || args.DeobfuscationType) {

		if (args.DecryptionType) {
			if (strcmp(args.DecryptionType, "xor") == 0)
				GenerateXorDecryptTemplate();
			else if (strcmp(args.DecryptionType, "aes") == 0)
				GenerateAesDecryptTemplate();
			else if (strcmp(args.DecryptionType, "chacha20") == 0)
				GenerateChaCha20DecryptTemplate();
			else if (strcmp(args.DecryptionType, "salsa20") == 0)
				GenerateSalsa20DecryptTemplate();
			else {
				fprintf(stderr, "[-] Unknown decryption type: %s\n", args.DecryptionType);
				return -1;
			}
		}

		if (args.DecodingType) {
			if (strcmp(args.DecodingType, "baseN") == 0)
				GenerateBaseNDecodeTemplate();
			else {
				fprintf(stderr, "[-] Unknown decoding type: %s\n", args.DecodingType);
				return -1;
			}
		}

		if (args.DeobfuscationType) {
			if (strcmp(args.DeobfuscationType, "uuid") == 0)
				GenerateUuidDeobfuscateTemplate();
			else if (strcmp(args.DeobfuscationType, "ipv6") == 0)
				GenerateIpv6DeobfuscateTemplate();
			else {
				fprintf(stderr, "[-] Unknown deobfuscation type: %s\n", args.DeobfuscationType);
				return -1;
			}
		}

		return 0;
	}

	// === VALIDATE ARGUMENTS ===
	if (!ValidateArguments(&args)) {
		return -1;
	}

	// === READ INPUT FILE ===
	PBYTE	pPayload = NULL;
	SIZE_T	sPayloadSize = 0;

	printf("[*] Mode: %s\n", args.bReadMode ? "READ (Console)" : "WRITE (File)");
	printf("[*] Input: %s\n", args.InputFilePath);

	if (!ReadPayloadFile(args.InputFilePath, &sPayloadSize, &pPayload)) {
		return -1;
	}

	printf("[+] File read: %zu bytes\n\n", sPayloadSize);

	// === EXECUTE OPERATIONS ===
	BOOL bHasObfuscation = (args.ObfuscationType != NULL);

	if (!ExecuteOperations(&args, &pPayload, &sPayloadSize)) {
		fprintf(stderr, "[-] Operations failed\n");
		HeapFree(GetProcessHeap(), 0, pPayload);
		return -1;
	}

	// If obfuscation was used, it already printed output
	if (bHasObfuscation) {
		// Calculate entropy
		double entropy = CalculateEntropy(pPayload, sPayloadSize);
		printf("[*] Shannon Entropy: %.4f ", entropy);
		if (entropy > 7.0)
			printf("(High - Suspicious)\n\n");
		else if (entropy > 6.0)
			printf("(Medium - Normal)\n\n");
		else
			printf("(Low - Stealthy)\n\n");

		HeapFree(GetProcessHeap(), 0, pPayload);
		return 0;
	}
	
	// === CALCULATE ENTROPY ===
	double entropy = CalculateEntropy(pPayload, sPayloadSize);

	printf("\n");
	printf("========================================\n");
	printf("SHANNON ENTROPY ANALYSIS\n");
	printf("========================================\n");
	printf("Entropy: %.4f bits/byte\n", entropy);
	printf("Status:  ");

	if (entropy > 7.0) {
		printf("High (Suspicious)\n");
		printf("Info: May trigger detection\n");
	}
	else if (entropy > 6.0) {
		printf("Medium (Normal)\n");
		printf("Info: Typical encrypted data\n");
	}
	else {
		printf("Low (Stealthy)\n");
		printf("Info: Appears less random\n");
	}

	printf("========================================\n\n");

	// === OUTPUT ===

	// WRITE MODE - Save binary to file
	if (args.bWriteMode) {
		printf("[*] Writing output: %s\n", args.OutputFileName);

		if (!WritePayloadFile(args.OutputFileName, sPayloadSize, pPayload)) {
			fprintf(stderr, "[-] Failed to write output\n");
			HeapFree(GetProcessHeap(), 0, pPayload);
			return -1;
		}

		printf("[+] Success! File saved: %s (%zu bytes)\n\n", args.OutputFileName, sPayloadSize);
	}
	// READ MODE - Print C array
	else {
		printf("========================================\n");
		printf("       PROCESSED PAYLOAD (C ARRAY)     \n");
		printf("========================================\n\n");

		PrintHexData("Payload", pPayload, sPayloadSize);
	}

	// === CLEANUP ===
	HeapFree(GetProcessHeap(), 0, pPayload);

	return 0;
}