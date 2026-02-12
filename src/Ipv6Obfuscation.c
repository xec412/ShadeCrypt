#include "Common.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------
* Generate Ipv6 Address Function
------------------------------------------------*/
char* GenerateIpv6Address(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	// Each Ipv6 segment comprises a total of 128 bits, meaning each ipv6 segment is 32 byte
	char Segment0[32], Segment1[32], Segment2[32], Segment3[32];

	// 32 * 4 = 128
	char* result = (char*)malloc(128);
	if (!result) return NULL;

	// Mixing segments to generate a ipv6-address
	sprintf_s(Segment0, sizeof(Segment0), "%0.2X%0.2X:%0.2X%0.2X", a, b, c, d);
	sprintf_s(Segment1, sizeof(Segment1), "%0.2X%0.2X:%0.2X%0.2X", e, f, g, h);
	sprintf_s(Segment2, sizeof(Segment2), "%0.2X%0.2X:%0.2X%0.2X", i, j, k, l);
	sprintf_s(Segment3, sizeof(Segment3), "%0.2X%0.2X:%0.2X%0.2X", m, n, o, p);

	// Combining all the segments to generate a complete ipv6-address
	sprintf_s(result, 128, "%s:%s:%s:%s", Segment0, Segment1, Segment2, Segment3);

	return (char*)result;
}

/*----------------------------------------------
* Generate Ipv6 Address Output Function
------------------------------------------------*/
BOOL GenerateIpv6Output(unsigned char* pData, SIZE_T sSize) {

	PBYTE					PaddedData = NULL;
	SIZE_T					PaddedSize = 0;

	if (!pData || sSize == 0) {
		printf("[!] Invalid Inputs \n");
		return FALSE;
	}

	// [!] In case file size is not a multiple of 16.
	if (sSize % 16 != 0) {
		PadBuffer(pData, sSize, &PaddedData, &PaddedSize);
		pData = PaddedData;
		sSize = PaddedSize;
	}

	printf("char* Ipv6Array[%d] = { \n\t", (int)(sSize / 16));

	int c = 16, counter = 0;
	char* IPV6 = NULL;

	for (SIZE_T i = 0; i < sSize; i++) {

		if (c == 16) {

			counter++;

			IPV6 = GenerateIpv6Address(
				pData[i], pData[i + 1], pData[i + 2], pData[i + 3],
				pData[i + 4], pData[i + 5], pData[i + 6], pData[i + 7],
				pData[i + 8], pData[i + 9], pData[i + 10], pData[i + 11],
				pData[i + 12], pData[i + 13], pData[i + 14], pData[i + 15]
			);

			if (i >= sSize - 16) {
				printf("\"%s\"", IPV6);
				free(IPV6);
				break;
			}
			else {
				printf("\"%s\", ", IPV6);
			}

			c = 1;
			free(IPV6);
			IPV6 = NULL;

			if (counter % 3 == 0) {
				printf("\n\t");
			}
		}
		else {
			c++;
		}
	}
	printf("\n};\n\n");

	if (PaddedData) HeapFree(GetProcessHeap(), 0, PaddedData);
	return TRUE;
}