// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces
//              to create and store a shortcut to the specified object.
//
// Returns the result of calling the member functions of the interfaces.
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszDesc     - Address of a buffer that contains a description of the
//                Shell link, stored in the Comment field of the link
//                properties.
#include <string>

#ifdef WIN32
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "shobjidl.h"
#include <cwchar>

PWSTR getProgramsFolder();

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszDesc)
{
    HRESULT hres;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            auto path = getProgramsFolder();
            wcsncpy (wsz, path, wcslen(path));
            wcsncpy (wsz, L"/mingw/MinGW_W64", wcslen(path));

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}

void shortcut(const std::string& file, const std::string& description)
{
    std::wstring wfile = std::wstring(file.begin(), file.end());
    std::wstring wdescription = std::wstring(description.begin(), description.end());
    CreateLink(wfile.c_str(), wdescription.c_str());
}

#else

void shortcut(const std::string& file, const std::string& description)
{
    printf("File to startup menu:  %s", file.c_str());
}

#endif