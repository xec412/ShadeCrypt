#include "Common.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------
* Generate Mac Address Function
------------------------------------------------*/
char* GenerateMacAddress(int a, int b, int c, int d, int e, int f) {

	char* result = (char*)malloc(64);
	if (!result) return NULL;

	sprintf_s(result, 64, "%0.2X-%0.2X-%0.2X-%0.2X-%0.2X-%0.2X", a, b, c, d, e, f);

	return (char*)result;
}

/*----------------------------------------------
* Generate Mac Address Output Function
------------------------------------------------*/
BOOL GenerateMacOutput(unsigned char* pData, SIZE_T sSize) {

	PBYTE				PaddedBuffer			= NULL;
	SIZE_T				PaddedSize				= 0;

	if (!pData || sSize == 0) {
		return FALSE;
	}

	// [!] In case file size is not a multiple of 6.
	if (sSize % 6 != 0) {
		PadMacBuffer(pData, sSize, &PaddedBuffer, &PaddedSize);
		pData = PaddedBuffer;
		sSize = PaddedSize;
	}

	printf("char* MacArray[%d] = { \n\t",(int)(sSize / 6));

	int c = 6, counter = 0;
	char* MAC = NULL;

	for (SIZE_T i = 0; i < sSize; i++) {

		if (c == 6) {

			counter++;

			MAC = GenerateMacAddress(
				pData[i], pData[i + 1], pData[i + 2],
				pData[i + 3], pData[i + 4], pData[i + 5]
			);

			if (i >= sSize - 6) {
				printf("\"%s\"", MAC);
				break;
			}
			else {
				printf("\"%s\", ", MAC);
			}

			c = 1;
			free(MAC);
			MAC = NULL;

			if (counter % 6 == 0) {
				printf("\n\t");
			}
		}
		else {
			c++;
		}
	}
	printf("\n};\n\n");
	if (PaddedBuffer) free(PaddedBuffer);
	return TRUE;
}