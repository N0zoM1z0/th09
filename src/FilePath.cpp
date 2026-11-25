#include "FileSystem.hpp"

#include <string.h>

namespace FileSystem
{
// These declarations describe target storage referenced by this function.
// Their defining translation unit and physical ownership remain unresolved.
char g_ResolvedPath[0x108];
char g_ExecutableDirectory[260];

char *__stdcall ResolvePath(const char *path)
{
    strcpy(g_ResolvedPath, g_ExecutableDirectory);
    strcat(g_ResolvedPath, path);
    return g_ResolvedPath;
}
} // namespace FileSystem
