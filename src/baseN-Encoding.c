#include <Windows.h>
#include <stdio.h>
#include "Common.h"

/*------------------------------------------------
 ## BaseN Encoding
--------------------------------------------------*/
VOID BaseNEncode(IN PBYTE InputBuffer, IN SIZE_T InputBufferSize, OUT PBYTE* ppOutputBuffer, OUT PSIZE_T psOutputSize, IN BOOL bEncode) {

	// Input validation: Ensure critical parameters are not NULL
	if (!InputBuffer || !InputBufferSize || !ppOutputBuffer) {
		fprintf(stderr, "[!] Invalid Inputs \n");
		return;
	}

	SIZE_T TmpSize = *psOutputSize = NULL;

	// Calculate memory requirements based on the direction (Encode vs Decode)
	if (bEncode)
		*psOutputSize = TmpSize = CALC_ENC_SIZE(ROUND_UP(InputBufferSize));
	else
		*psOutputSize = TmpSize = (BASE_N * InputBufferSize - (BASE_N * 8 - 4)) / 8;

	// Dynamic allocation of the output buffer from the process heap
	*ppOutputBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, TmpSize);
	if (!*ppOutputBuffer) return (VOID)ReportError("HeapAlloc");

	// Pointers for navigating the input and output streams
	unsigned char* TmpOutputBuffer = (unsigned char*)*ppOutputBuffer;
	unsigned char* TmpInputBuffer = (unsigned char*)InputBuffer;

	// Bit-streaming state variables
	unsigned int X = 0, Z = 0; // X: Bit accumulator Z: Bit counter
	BYTE WL = 8, RL = BASE_N, MV = (1 << 8) - 1; // WL: Window, RL: Read Length, MV: Bit-mask

	// Re-initialize streaming logic for Encoding direction
	if (bEncode) {
		RL = 8;                         // Reading 8-bit bytes from source
		WL = BASE_N;                    // Writing chunks of BASE_N bits
		MV = (1 << BASE_N) - 1;         // Mask to extract exactly BASE_N bits
	}

	// Main processing loop: Continue until the output buffer is filled
	while (TmpSize) {

		// Push input bits into the accumulator and update the bit counter
		X = (X << RL) | *TmpInputBuffer++;
		Z += RL;

		// While there are enough bits in the accumulator to form an output chunk
		while (Z >= WL) {
			Z -= WL;                            // Subtract window length from counter
			*TmpOutputBuffer++ = (X >> Z) & MV; // Mask and extract bits to the output stream
			TmpSize--;                          // Decrement remaining output size
		}
	}
}