#include <Windows.h>
#include <stdio.h>
#include "Common.h"

/*-------------------------------------------------
 ## Generate Ipv6 Address
--------------------------------------------------*/
CHAR* GenerateIpv6Address(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	// Temporary buffers for hexadecimal colon-separated segments
	CHAR seg0[32], seg1[32], seg2[32], seg3[32];

	// Allocate memory for the final IPv6 string (128 bytes provides ample overhead)
	CHAR* result = (CHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 128);
	if (!result) {
		fprintf(stderr, "[!] HeapAlloc Failed \n");
		return NULL;
	}

	// Formatting segments: Each block represents 2 bytes in hexadecimal
	sprintf_s(seg0, 32, "%0.2X%0.2X:%0.2X%0.2X", a, b, c, d);
	sprintf_s(seg1, 32, "%0.2X%0.2X:%0.2X%0.2X", e, f, g, h);
	sprintf_s(seg2, 32, "%0.2X%0.2X:%0.2X%0.2X", i, j, k, l);
	sprintf_s(seg3, 32, "%0.2X%0.2X:%0.2X%0.2X", m, n, o, p);

	// Finalize string by concatenating the hex segments into a full IPv6 address
	sprintf_s(result, 128, "%s:%s:%s:%s", seg0, seg1, seg2, seg3);

	return (CHAR*)result;
}

/*-------------------------------------------------
 ## Generate Ipv6 Address Output
--------------------------------------------------*/
BOOL GenerateIpv6Output(IN PBYTE pPayload, IN SIZE_T sPayloadSize) {

	PBYTE			PaddedBuffer		= NULL;
	SIZE_T			PaddedSize			= 0;

	// Input validation: Ensure payload integrity before processing
	if (pPayload == NULL || sPayloadSize == 0) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return FALSE;
	}

	// Alignment: Ensure the payload is padded to a 16-byte boundary for IPv6 conversion
	if (sPayloadSize % 16 != 0) {
		if (!PadBuffer(pPayload, sPayloadSize, &PaddedBuffer, &PaddedSize)) {
			return FALSE;
		}
		pPayload = PaddedBuffer;
		sPayloadSize = PaddedSize;
	}

	// Begin printing the formatted C-style array for the header/source file
	printf("CHAR* Ipv6Array[%d] = { \n\t", (int)(sPayloadSize / 16));

	int c = 16, counter = 0;
	CHAR* IPV6 = NULL;

	// Iterate through the payload in 16-byte increments
	for (int i = 0; i < sPayloadSize; i++) {

		if (c == 16) {

			counter++;

			// Transform the current 16-byte block into an IPv6 string
			IPV6 = GenerateIpv6Address(
				pPayload[i], pPayload[i + 1], pPayload[i + 2], pPayload[i + 3],
				pPayload[i + 4], pPayload[i + 5], pPayload[i + 6], pPayload[i + 7],
				pPayload[i + 8], pPayload[i + 9], pPayload[i + 10], pPayload[i + 11],
				pPayload[i + 12], pPayload[i + 13], pPayload[i + 14], pPayload[i + 15]
			);

			// Logic to manage trailing commas for proper C array syntax
			if (i == sPayloadSize - 16) {
				printf("\"%s\"", IPV6);
			}
			else {
				printf("\"%s\", ", IPV6);
			}

			c = 1;
			// Clean up the temporary IPv6 string buffer
			HeapFree(GetProcessHeap(), 0, IPV6);
			IPV6 = NULL;

			// Formatting: Add a newline every 3 elements to maintain code readability
			if (counter % 3 == 0) {
				printf("\n\t");
			}
		}
		else {
			c++;
		}
	}

	printf("\n};\n\n");

	// Deallocate the padding buffer if it was utilized
	if (PaddedBuffer) HeapFree(GetProcessHeap(), 0, PaddedBuffer);

	return TRUE;
}