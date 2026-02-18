# ShadeCrypt

A modular payload encryption, encoding, and obfuscation tool for security research and red team operations.

## Overview

ShadeCrypt is a command-line utility designed to transform binary payloads through multiple cryptographic and obfuscation layers. Inspired by techniques from [MalDevAcademy](https://maldevacademy.com) and their [HellShell](https://github.com/maldevacademy/hellshell) project, this tool provides a practical implementation of payload transformation workflows commonly used in offensive security research.

## Features

### Encryption Algorithms
- **XOR Cipher** - Multi-byte key XOR encryption
- **AES-256-CBC** - Industry-standard block cipher with 256-bit key
- **ChaCha20** - Modern stream cipher (32-byte key, 12-byte IV)
- **Salsa20** - High-speed stream cipher (32-byte key, 8-byte nonce)

### Encoding
- **BaseN Encoding** - Custom bit-streaming algorithm for data transformation

### Obfuscation
- **UUID String Arrays** - Convert binary data to RFC 4122-compliant UUID strings
- **IPv6 Address Arrays** - Transform payloads into valid IPv6 address representations

### Additional Capabilities
- **Entropy Calculation** - Shannon entropy analysis to measure randomness
- **Template Generation** - Auto-generate C code for decryption/deobfuscation routines
- **Modular Pipeline** - Combine operations (encrypt → encode → obfuscate)
- **Flexible Output** - Console display (C arrays) or binary file export

## Building

### Prerequisites
- Windows SDK
- Visual Studio 2019+ or MinGW-w64
- C compiler with C11 support

### Compilation

**Visual Studio:**
```cmd
cl /I"include" /I"lib" src\*.c lib\*.c /Fe:ShadeCrypt.exe /link bcrypt.lib
```

**MinGW:**
```bash
gcc -Iinclude -Ilib src/*.c lib/*.c -o ShadeCrypt.exe -lbcrypt
```

## Usage

### Basic Syntax
```
ShadeCrypt.exe --file <path> [--encrypt <algo>] [--encode <method>] [--obfuscate <type>]
```

### Examples

**Simple XOR encryption:**
```bash
ShadeCrypt.exe --file payload.bin --encrypt xor
```

**AES encryption with UUID obfuscation:**
```bash
ShadeCrypt.exe --file payload.bin --encrypt aes --obfuscate uuid
```

**Full pipeline (encrypt → encode → obfuscate):**
```bash
ShadeCrypt.exe --file payload.bin --encrypt chacha20 --encode baseN --obfuscate ipv6
```

**Save encrypted output to file:**
```bash
ShadeCrypt.exe --write --file payload.bin --encrypt salsa20 --output encrypted.bin
```

**Generate decryption template:**
```bash
ShadeCrypt.exe --decrypt aes
```

### Output Modes

**Console Mode (default):**
- Displays processed payload as C array
- Shows encryption keys/IVs for use in loaders
- Calculates Shannon entropy

**File Mode (`--write`):**
- Saves binary output to disk
- Requires `--output` parameter

### Template Generation

ShadeCrypt can generate ready-to-use C code for decryption/deobfuscation:

```bash
# Generate AES decryption function
ShadeCrypt.exe --decrypt aes

# Generate UUID deobfuscation function
ShadeCrypt.exe --deobfuscate uuid

# Generate BaseN decoding function
ShadeCrypt.exe --decode baseN
```

## Project Structure

```
ShadeCrypt/
├── include/
│   ├── Common.h           # Function prototypes and constants
│   ├── ArgParser.h        # Command-line argument structures
│   ├── Operations.h       # Operation pipeline handler
│   └── Templates.h        # Template generation prototypes
├── src/
│   ├── ShadeCrypt.c       # Main entry point
│   ├── ArgParser.c        # Argument parsing and validation
│   ├── Operations.c       # Encryption/encoding/obfuscation pipeline
│   ├── Templates.c        # Decryption template generators
│   ├── Utils.c            # File I/O, entropy, helpers
│   ├── XorEncryption.c    # XOR cipher implementation
│   ├── AesEncryption.c    # AES-256-CBC wrapper
│   ├── ChaChaEncryption.c # ChaCha20 wrapper
│   ├── SalsaEncryption.c  # Salsa20 wrapper
│   ├── baseN-Encoding.c   # Custom BaseN algorithm
│   ├── UuidObfuscation.c  # UUID string generation
│   └── Ipv6Obfuscation.c  # IPv6 address generation
└── lib/
    ├── aes.c/h            # Tiny-AES implementation
    ├── ChaCha.c/h         # ChaCha20 library
    └── Salsa20.c/h        # Salsa20 library
```

## Entropy Analysis

ShadeCrypt automatically calculates Shannon entropy for processed payloads:

- **< 6.0** - Low entropy (potentially stealthy)
- **6.0 - 7.0** - Medium entropy (typical for encrypted data)
- **> 7.0** - High entropy (may trigger suspicion)

This metric helps assess how "random" the output appears, which can be relevant when considering detection heuristics.

## Educational Context

This tool was developed as a learning project while studying offensive security techniques. Key concepts implemented:

- **Cryptographic primitives** - Understanding encryption algorithm requirements
- **Memory management** - Safe heap allocation and cleanup patterns
- **Modular architecture** - Separating concerns (parsing, operations, output)
- **Windows API** - Working with bcrypt.lib for cryptographically secure RNG
- **Code generation** - Producing valid C code programmatically

## Credits & Acknowledgments

- **MalDevAcademy** - For comprehensive malware development education and the HellShell project that inspired this tool's architecture
- **Tiny-AES** - For the lightweight AES implementation
- **LibTomCrypt** - For ChaCha20 and Salsa20 reference implementations

## Disclaimer

This tool is intended exclusively for:
- Educational purposes
- Authorized security research
- Legitimate red team engagements with proper authorization

**The author assumes no liability for misuse. Always obtain explicit written permission before testing security controls.**

## License

MIT License - See LICENSE file for details

## Author

**Xec0xCC** - Offensive security student specializing in malware development and Windows internals

---

*Built with techniques learned from [MalDevAcademy](https://maldevacademy.com)*
