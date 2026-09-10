#pragma once

#include <stddef.h>

namespace FileSystem
{
char *__stdcall ResolvePath(const char *path);
unsigned char *Decrypt(unsigned char *inData, int size, unsigned char xorValue,
                       unsigned char xorValueInc, int chunkSize, int maxBytes);
unsigned char *OpenFile(const char *path, int *fileSize, int isExternalResource);
int CheckIfFileAlreadyExists(const char *path);
int WriteDataToFile(const char *path, void *data, size_t size);
}
