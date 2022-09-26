#ifdef WIN32
#include <Windows.h>
#include <fstream>
#include <shlobj.h>
#include <string>
#include <KnownFolders.h>
#include <cwchar>


PWSTR getProgramsFolder()
{
    PWSTR path = NULL;
    HRESULT hres = SHGetKnownFolderPath(FOLDERID_CommonPrograms, 0, NULL, &path);
    return path;
}
#endif