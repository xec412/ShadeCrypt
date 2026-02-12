#include "Common.h"
#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
 * Display tool usage instructions.
--------------------------------------------------------------------*/
void PrintUsage(const char* procName) {
    printf("\n");
    printf("   _____ __              __     ______                  __\n");
    printf("  / ___// /_  ____ _____/ /__  / ____/______  ______  / /_\n");
    printf("  \\__ \\/ __ \\/ __ `/ __  / _ \\/ /   / ___/ / / / __ \\/ __/\n");
    printf(" ___/ / / / / /_/ / /_/ /  __/ /___/ /  / /_/ / /_/ / /_  \n");
    printf("/____/_/ /_/\\__,_/\\__,_/\\___/\\____/_/   \\__, / .___/\\__/  \n");
    printf("                                       /____/_/            \n\n");
    printf("====================================================================\n");
    printf("  Payload Encryption & Obfuscation Tool\n");
    printf("  AES-256 | ChaCha20 | XOR | UUID | IPv6 | MAC\n");
    printf("====================================================================\n\n");
    printf("[!] Usage: %s <input_file> <action> <method>\n\n", procName);
    printf("--------------------------------------------------------------------\n");
    printf("[+] Actions:\n");
    printf("    encrypt       : Encrypt or obfuscate input\n");
    printf("    decrypt       : Generate decryption template\n\n");
    printf("[+] Methods:\n");
    printf("    Encryption    : xor | aes | chacha\n");
    printf("    Obfuscation   : ipv6 | uuid | mac\n");
    printf("--------------------------------------------------------------------\n");
    printf("\n[*] Examples:\n");
    printf("    %s payload.bin encrypt xor\n", procName);
    printf("    %s payload.bin encrypt ipv6\n", procName);
    printf("    %s payload.bin decrypt uuid\n", procName);
    printf("====================================================================\n\n");
}

/*--------------------------------------------------------------------
 * Main Function
--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {

    // Checking argument count
    if (argc < 4) {
        PrintUsage(argv[0]);
        return -1;
    }

    PBYTE Data = NULL;
    DWORD Size = 0;
    BOOL bDecrypt = FALSE;

    // Check if decrypt/template mode
    if (_stricmp(argv[2], "decrypt") == 0 || _stricmp(argv[2], "deobfuscate") == 0) {
        bDecrypt = TRUE;
    }

    /*--------------------------------------------------------------------
    * Template Generation (Decrypt Mode)
    --------------------------------------------------------------------*/
    if (bDecrypt) {
        printf("\n[+] Generating template for: %s \n\n", argv[3]);

        if (_stricmp(argv[3], "uuid") == 0) {
            printf("%s", UUID_TEMPLATE);
        }
        else if (_stricmp(argv[3], "ipv6") == 0) {
            printf("%s", IPV6_TEMPLATE);
        }
        else if (_stricmp(argv[3], "mac") == 0) {
            printf("%s", MAC_TEMPLATE);
        }
        else if (_stricmp(argv[3], "aes") == 0) {
            printf("%s", AES_TEMPLATE);
        }
        else if (_stricmp(argv[3], "xor") == 0) {
            printf("%s", XOR_TEMPLATE);
        }
        else if (_stricmp(argv[3], "chacha") == 0 || _stricmp(argv[3], "chacha20") == 0) {
            printf("%s", CHACHA_TEMPLATE);
        }
        else if (_stricmp(argv[3], "salsa") == 0 || _stricmp(argv[3], "salsa20") == 0) {
            printf("%s", SALSA_TEMPLATE);
        }
        else {
            printf("[!] Unknown method: %s \n", argv[3]);
            return -1;
        }
        return 0;
    }

    /*--------------------------------------------------------------------
    * Encryption / Obfuscation Mode
    --------------------------------------------------------------------*/
    printf("\n[+] Reading file: %s \n", argv[1]);
    if (!ReadBinaryFile(argv[1], &Data, &Size)) {
        printf("[!] Failed to read file\n");
        return -1;
    }
    printf("[+] File size: %d bytes \n\n", Size);

    if (_stricmp(argv[3], "xor") == 0) {
        printf("[+] XOR Encryption \n");
        BYTE Key[KeyLength];
        GenerateRandomBytes(Key, KeyLength);
        PrintHexData("XorKey", Key, KeyLength);
        XorEncrypt(Data, Size, Key, sizeof(Key));
        PrintHexData("XorCiphered", Data, Size);
    }
    else if (_stricmp(argv[3], "aes") == 0) {
        printf("[+] AES-256 Encryption \n");
        BYTE Key[KeyLength];
        BYTE Iv[IvLength];
        GenerateRandomBytes(Key, KeyLength);
        GenerateRandomBytes(Iv, IvLength);
        PrintHexData("AesKey", Key, KeyLength);
        PrintHexData("AesIv", Iv, IvLength);
        Aes_Encrypt(Data, Size, Key, Iv);
        PrintHexData("AesCiphered", Data, Size);
    }
    else if (_stricmp(argv[3], "chacha") == 0 || _stricmp(argv[3], "chacha20") == 0) {
        printf("[+] ChaCha20 Encryption \n");
        BYTE Key[KeyLength];
        BYTE Iv[ChaChaIvLength];
        GenerateRandomBytes(Key, KeyLength);
        GenerateRandomBytes(Iv, ChaChaIvLength);
        PrintHexData("ChaChaKey", Key, KeyLength);
        PrintHexData("ChaChaIv", Iv, ChaChaIvLength);
        ChaCha20Encrypt(Data, Size, Key, Iv);
        PrintHexData("ChaCha20Ciphered", Data, Size);
    }
    else if (_stricmp(argv[3], "salsa") == 0 || _stricmp(argv[3], "salsa20") == 0) {
        printf("[+] Salsa20 Encryption \n");
        BYTE Key[KeyLength];
        BYTE Nonce[SalsaNonceLength];
        GenerateRandomBytes(Key, KeyLength);
        GenerateRandomBytes(Nonce, SalsaNonceLength);
        PrintHexData("SalsaKey", Key, KeyLength);
        PrintHexData("SalsaNonce", Nonce, SalsaNonceLength);
        Salsa20Encrypt(Data, Size, Key, Nonce);
        PrintHexData("Salsa20Ciphered", Data, Size);
    }
    else if (_stricmp(argv[3], "uuid") == 0) {
        printf("[+] UUID Obfuscation\n");
        GenerateUuidOutput(Data, Size);
    }
    else if (_stricmp(argv[3], "ipv6") == 0) {
        printf("[+] IPv6 Obfuscation\n");
        GenerateIpv6Output(Data, Size);
    }
    else if (_stricmp(argv[3], "mac") == 0) {
        printf("[+] MAC Address Obfuscation\n");
        GenerateMacOutput(Data, Size);
    }
    else {
        printf("[!] Unknown method: %s\n", argv[3]);
        PrintUsage(argv[0]);
        if (Data) HeapFree(GetProcessHeap(), 0, Data);
        return -1;
    }

    if (Data) {
        HeapFree(GetProcessHeap(), 0, Data);
    }

    printf("\n[+] Done!\n\n");
    return 0;
}