// Operations.h
#pragma once

#include <Windows.h>
#include "ArgParser.h"

// Execute all operations based on parsed arguments
BOOL ExecuteOperations(IN PARGS pArgs, IN OUT PBYTE* ppPayload, IN OUT PSIZE_T psPayloadSize);