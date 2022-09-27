#ifdef WIN32
#include <Windows.h>
#include <fstream>
#include <shlobj.h>
#include <string>
#include <KnownFolders.h>
#include <cwchar>


std::wstring getProgramsFolder()
{
    wchar_t path[MAX_PATH];
    wchar_t* p = path;
    HRESULT hres = SHGetKnownFolderPath(FOLDERID_CommonPrograms, 0, NULL, &p);
    return std::wstring{path};
}

#endif