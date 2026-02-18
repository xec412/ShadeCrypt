#include <Windows.h>
#include <stdio.h>
#include "Common.h"

/*-------------------------------------------------
 ## Generate UUID-Formatted String
--------------------------------------------------*/
CHAR* GenerateUuidString(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	// Temporary buffers for hexadecimal segment representation
	CHAR seg0[32], seg1[32], seg2[32], seg3[32];

	// Dynamically allocate memory for the final UUID string (UTF-8/ASCII)
	CHAR* result = (CHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 128);
	if (!result) {
		fprintf(stderr, "[!] HeapAlloc Failed \n");
		return NULL;
	}

	// Formatting segments: Mapping raw bytes to the hex-string UUID structure
	// Segments 0, 1, and 2 follow little-endian ordering for binary-to-string conversion
	sprintf_s(seg0, 32, "%0.2X%0.2X%0.2X%0.2X", d, c, b, a);
	sprintf_s(seg1, 32, "%0.2X%0.2X-%0.2X%0.2X", f, e, h, g);
	sprintf_s(seg2, 32, "%0.2X%0.2X-%0.2X%0.2X", i, j, k, l);
	sprintf_s(seg3, 32, "%0.2X%0.2X%0.2X%0.2X", m, n, o, p);

	// Finalize string by concatenating formatted hex segments with hyphens
	sprintf_s(result, 128, "%s-%s-%s%s", seg0, seg1, seg2, seg3);

	return (CHAR*)result;
}

/*-------------------------------------------------
 ## Generate UUID-Formatted Output
--------------------------------------------------*/
BOOL GenerateUuidOutput(IN PBYTE pPayload, IN SIZE_T sPayloadSize) {

	PBYTE			PaddedBuffer = NULL;
	SIZE_T			PaddedSize = 0;

	// Input validation: Ensure payload exists and has content
	if (pPayload == NULL || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return FALSE;
	}

	// Alignment: Payloads must be a multiple of 16 bytes for UUID conversion
	if (sPayloadSize % 16 != 0) {
		if (!PadBuffer(pPayload, sPayloadSize, &PaddedBuffer, &PaddedSize)) {
			return FALSE;
		}
		pPayload = PaddedBuffer;
		sPayloadSize = PaddedSize;
	}

	// Begin printing the formatted C-style array
	printf("CHAR* UuidArray[%d] = { \n\t", (int)(sPayloadSize / 16));

	int c = 16, counter = 0;
	CHAR* UUID = NULL;

	// Iterate through the payload in 16-byte chunks
	for (int i = 0; i < sPayloadSize; i++) {

		if (c == 16) {

			counter++;

			// Transform the current 16-byte block into a UUID string
			UUID = GenerateUuidString(
				pPayload[i], pPayload[i + 1], pPayload[i + 2], pPayload[i + 3],
				pPayload[i + 4], pPayload[i + 5], pPayload[i + 6], pPayload[i + 7],
				pPayload[i + 8], pPayload[i + 9], pPayload[i + 10], pPayload[i + 11],
				pPayload[i + 12], pPayload[i + 13], pPayload[i + 14], pPayload[i + 15]
			);

			// Logic to handle trailing commas in the array output
			if (i == sPayloadSize - 16) {
				printf("\"%s\"", UUID);
			}
			else {
				printf("\"%s\", ", UUID);
			}

			c = 1;
			// Crucial: Free the allocated UUID string buffer to prevent leaks
			HeapFree(GetProcessHeap(), 0, UUID);
			UUID = NULL;

			// Visual formatting: Newline after every 3 elements for readability
			if (counter % 3 == 0) {
				printf("\n\t");
			}
		}
		else {
			c++;
		}
	}

	printf("\n};\n\n");

	// Clean up the padding buffer if one was created
	if (PaddedBuffer) HeapFree(GetProcessHeap(), 0, PaddedBuffer);

	return TRUE;
}