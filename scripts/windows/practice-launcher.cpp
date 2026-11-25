#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

namespace
{
// Fixed addresses from the reviewed trial-playable.map at image base 0x400000.
const DWORD CollisionDispatch = 0x00419256;
const DWORD NoHit = 0x00419277;
const DWORD CollisionOwner = 0x0041925D;
const BYTE ExpectedDispatch[] = {0x85, 0xC0, 0x74, 0x1D, 0x8B, 0x40, 0x2C};
const char ExpectedSha256[] = "6f28f7e6aa44529e61112c46b0ca5befc82598faf8675a964edd527ef94348e5";

bool VerifyImage(HANDLE file)
{
    HCRYPTPROV provider = 0;
    HCRYPTHASH hash = 0;
    BYTE buffer[16 * 1024];
    BYTE digest[32];
    DWORD count, size = sizeof(digest);
    bool valid = false;
    if (CryptAcquireContextW(&provider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT) &&
        CryptCreateHash(provider, CALG_SHA_256, 0, 0, &hash))
    {
        while (ReadFile(file, buffer, sizeof(buffer), &count, NULL) &&
               (count == 0 || CryptHashData(hash, buffer, count, 0)))
        {
            if (count == 0)
            {
                char hex[65];
                if (CryptGetHashParam(hash, HP_HASHVAL, digest, &size, 0) && size == sizeof(digest))
                {
                    for (unsigned int i = 0; i < sizeof(digest); ++i)
                        sprintf(hex + 2 * i, "%02x", static_cast<unsigned int>(digest[i]));
                    valid = strcmp(hex, ExpectedSha256) == 0;
                }
                break;
            }
        }
    }
    if (hash)
        CryptDestroyHash(hash);
    if (provider)
        CryptReleaseContext(provider, 0);
    return valid;
}

void Rel32(BYTE *operand, DWORD next, DWORD target)
{
    const DWORD displacement = target - next;
    memcpy(operand, &displacement, sizeof(displacement));
}

bool MatchesMemory(HANDLE process, DWORD address, const BYTE *expected, SIZE_T size)
{
    BYTE actual[128];
    SIZE_T count = 0;
    return size <= sizeof(actual) &&
           ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), actual, size, &count) &&
           count == size && memcmp(actual, expected, size) == 0;
}

bool WriteAndVerify(HANDLE process, DWORD address, const BYTE *data, SIZE_T size)
{
    SIZE_T count = 0;
    return WriteProcessMemory(process, reinterpret_cast<LPVOID>(address), data, size, &count) &&
           count == size && MatchesMemory(process, address, data, size);
}

bool InstallPracticePatch(HANDLE process)
{
    if (!MatchesMemory(process, CollisionDispatch, ExpectedDispatch, sizeof(ExpectedDispatch)))
        return false;

    // ESI is the player and EAX is the collision query's result. Let local P1
    // play the first round of the story finale with the original damage rules.
    // Other local practice hits follow the no-hit path before marking the owner.
    enum { ProtectPlayer = 63, RestoreStock = 68, Stock = 69 };
    BYTE code[] = {
        0x83, 0x7E, 0x08, 0x00,                   // cmp dword ptr [esi+8], 0
        0x75, Stock - 6,                          // jne stock
        0xF6, 0x05, 0x14, 0x70, 0x4A, 0x00, 0x0A, // test byte ptr [g_GameManager+134h], 0Ah
        0x75, Stock - 15,                         // jne stock
        0x52,                                     // push edx
        0x8B, 0x15, 0x20, 0x34, 0x4B, 0x00,       // mov edx, [g_SupervisorNetworkState]
        0x85, 0xD2,                               // test edx, edx
        0x74, RestoreStock - 26,                  // jz restore_stock
        0x83, 0xBA, 0xA8, 0x00, 0x00, 0x00, 0x00, // cmp dword ptr [edx+0A8h], 0
        0x5A,                                     // pop edx
        0x75, Stock - 36,                         // jne stock
        0x83, 0x3D, 0xF8, 0x6F, 0x4A, 0x00, 0x00, // cmp dword ptr [g_GameManager+118h], 0
        0x75, ProtectPlayer - 45,                 // jne protect_player
        0x83, 0x3D, 0xDC, 0x6F, 0x4A, 0x00, 0x08, // cmp dword ptr [g_GameStageValue], 8
        0x75, ProtectPlayer - 54,                 // jne protect_player
        0x83, 0x3D, 0xE0, 0x6F, 0x4A, 0x00, 0x00, // cmp dword ptr [g_FrontRuntimeValues90], 0
        0x74, Stock - 63,                         // je stock
        0xE9, 0, 0, 0, 0,                         // protect_player: jmp NoHit
        0x5A,                                     // restore_stock: pop edx
        0x85, 0xC0,                               // stock: test eax, eax
        0x0F, 0x84, 0, 0, 0, 0,                   // jz NoHit
        0x8B, 0x40, 0x2C,                         // mov eax, [eax+2Ch]
        0xE9, 0, 0, 0, 0,                         // jmp CollisionOwner
    };
    LPVOID region = VirtualAllocEx(process, NULL, sizeof(code), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!region)
        return false;
    const DWORD base = reinterpret_cast<DWORD>(region);
    Rel32(code + ProtectPlayer + 1, base + ProtectPlayer + 5, NoHit);
    Rel32(code + Stock + 4, base + Stock + 8, NoHit);
    Rel32(code + sizeof(code) - 4, base + sizeof(code), CollisionOwner);
    DWORD savedProtection;
    if (!WriteAndVerify(process, base, code, sizeof(code)) ||
        !VirtualProtectEx(process, region, sizeof(code), PAGE_EXECUTE_READ, &savedProtection) ||
        !FlushInstructionCache(process, region, sizeof(code)))
        return false;

    BYTE jump[] = {0xE9, 0, 0, 0, 0, 0x90, 0x90};
    Rel32(jump + 1, CollisionDispatch + 5, base);
    LPVOID site = reinterpret_cast<LPVOID>(CollisionDispatch);
    if (!VirtualProtectEx(process, site, sizeof(jump), PAGE_EXECUTE_READWRITE, &savedProtection))
        return false;
    const bool wrote = WriteAndVerify(process, CollisionDispatch, jump, sizeof(jump));
    DWORD ignored;
    const bool restored = VirtualProtectEx(process, site, sizeof(jump), savedProtection, &ignored) != FALSE;
    return wrote && restored && FlushInstructionCache(process, site, sizeof(jump));
}
}

int wmain(int argc, wchar_t **argv)
{
    const bool checkOnly = argc == 2 && wcscmp(argv[1], L"--check") == 0;
    if (argc != 1 && !checkOnly)
    {
        fputs("Usage: th09-practice.exe [--check]\n", stderr);
        return 1;
    }
    wchar_t folder[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, folder, MAX_PATH);
    if (!length || length >= MAX_PATH)
        return 1;
    wchar_t *separator = wcsrchr(folder, L'\\');
    if (!separator)
        return 1;
    *separator = L'\0';
    wchar_t game[MAX_PATH];
    if (_snwprintf(game, MAX_PATH, L"%ls\\th09-reconstructed.exe", folder) < 0)
        return 1;

    HANDLE file = CreateFileW(game, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE || !VerifyImage(file))
    {
        if (file != INVALID_HANDLE_VALUE)
            CloseHandle(file);
        fputs("The adjacent th09-reconstructed.exe differs from the reviewed playable build.\n", stderr);
        return 1;
    }
    if (checkOnly)
    {
        CloseHandle(file);
        puts("Reviewed TH09 playable executable verified.");
        return 0;
    }

    STARTUPINFOW startup = {};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION child = {};
    if (!CreateProcessW(game, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, folder, &startup, &child))
    {
        CloseHandle(file);
        fprintf(stderr, "Could not start the reviewed game (Win32 error %lu).\n", GetLastError());
        return 1;
    }
    const bool ready = InstallPracticePatch(child.hProcess) &&
                       ResumeThread(child.hThread) == 1;
    CloseHandle(file);
    if (!ready)
    {
        TerminateProcess(child.hProcess, 1);
        fputs("Practice patch verification failed. The suspended game was closed.\n", stderr);
    }
    else
    {
        printf("Local P1 practice enabled in process %lu.\n", child.dwProcessId);
    }
    CloseHandle(child.hThread);
    CloseHandle(child.hProcess);
    return ready ? 0 : 1;
}
