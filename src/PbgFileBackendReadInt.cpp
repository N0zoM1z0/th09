#include "PbgFileBackend.hpp"

DWORD PbgFileBackend::ReadInt(int *outData)
{
    return Read(outData, 4);
}
