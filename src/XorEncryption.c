#include "Common.h"
#include <stdio.h>
#include <stdlib.h>

/*----------------------------------------------
* Xor Encrypt Function
------------------------------------------------*/
VOID XorEncrypt(IN PBYTE Data, IN SIZE_T Size, IN PBYTE Key, IN SIZE_T KeySize) {

	for (size_t i = 0, j = 0; i < Size; i++, j++) {
		if (j >= KeySize) {
			j = 0;
		}
		Data[i] = Data[i] ^ Key[j];
	}
}