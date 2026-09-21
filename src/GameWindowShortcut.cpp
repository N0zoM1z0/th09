#include <windows.h>
#include <shlobj.h>

int __fastcall ResolveShortcut(
    char *shortcutPath,
    char *destination,
    int destinationSize)
{
    HRESULT result;
    IPersistFile *persistFile;
    IShellLinkA *shellLink;
    int returnValue;
    WIN32_FIND_DATAA findData;
    LPWSTR widePath;

    if (destination == NULL)
        return FALSE;

    returnValue = FALSE;
    CoInitialize(NULL);
    result = CoCreateInstance(
        CLSID_ShellLink,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IShellLinkA,
        reinterpret_cast<void **>(&shellLink));
    if (SUCCEEDED(result))
    {
        result = shellLink->QueryInterface(
            IID_IPersistFile,
            reinterpret_cast<void **>(&persistFile));
        if (SUCCEEDED(result))
        {
            widePath = new WCHAR[destinationSize];
            MultiByteToWideChar(
                CP_ACP, 0, shortcutPath, -1, widePath, destinationSize);
            result = persistFile->Load(widePath, STGM_READ);
            if (SUCCEEDED(result))
            {
                result = shellLink->GetPath(
                    destination, destinationSize, &findData, 0);
                if (SUCCEEDED(result))
                    returnValue = TRUE;
            }

            delete widePath;
            persistFile->Release();
        }
        shellLink->Release();
    }
    CoUninitialize();
    return returnValue;
}
