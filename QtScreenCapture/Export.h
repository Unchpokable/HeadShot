#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "Windows.h"

typedef struct TransferArray__
{
    PVOID Data;
    ULONGLONG DataLength;
} TransferArray, *PTransferArray;

__declspec(dllexport) TransferArray MakeQtScreenshot(int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif