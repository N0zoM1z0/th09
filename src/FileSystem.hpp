#pragma once

#include <stddef.h>

namespace FileSystem
{
char *__stdcall ResolvePath(const char *path);
int CheckIfFileAlreadyExists(const char *path);
int WriteDataToFile(const char *path, void *data, size_t size);
}
