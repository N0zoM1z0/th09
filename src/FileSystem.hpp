#pragma once

#include <stddef.h>

namespace FileSystem
{
int WriteDataToFile(const char *path, void *data, size_t size);
}
