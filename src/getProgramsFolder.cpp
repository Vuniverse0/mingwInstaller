#ifdef WIN32
#include <Windows.h>
#include <fstream>
#include <shlobj.h>
#include <string>
#include <KnownFolders.h>
#include <cwchar>


std::wstring getProgramsFolder()
{
    wchar_t* p = nullptr;
    HRESULT hres = SHGetKnownFolderPath(FOLDERID_ApplicationShortcuts, 0, NULL, &p);
    return std::wstring{p};
}

#endif