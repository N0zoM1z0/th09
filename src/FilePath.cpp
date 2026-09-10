#include "FileSystem.hpp"

#include <string.h>

namespace FileSystem
{
// These declarations describe target storage referenced by this function.
// Their defining translation unit and physical ownership remain unresolved.
extern char g_ExecutableDirectory[];
extern char g_ResolvedPath[];

char *__stdcall ResolvePath(const char *path)
{
    strcpy(g_ResolvedPath, g_ExecutableDirectory);
    strcat(g_ResolvedPath, path);
    return g_ResolvedPath;
}
} // namespace FileSystem
