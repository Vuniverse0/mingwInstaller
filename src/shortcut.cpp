#include <string>
#include <locale>
#include <codecvt>
#include <algorithm>
#ifdef WIN32
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "shobjidl.h"
#include <Windows.h>
#include <fstream>
#include <shlobj.h>
#include <KnownFolders.h>
#include <cwchar>

/*============================================================================*/

// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces
//              to create and store a shortcut to the specified object.
//
// Returns the result of calling the member functions of the interfaces.
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszPathLink - Address of a buffer that contains the path where the
//                Shell link is to be stored, including the file name.
// lpszDesc     - Address of a buffer that contains a description of the
//                Shell link, stored in the Comment field of the link
//                properties.
//lpszPicture   - picture of shortcut

HRESULT CreateLink(LPCSTR lpszPathObj, LPCWSTR lpszPathLink,
                   LPCSTR lpszDesc, LPCSTR lpszPicture)
{
    CoInitializeEx( NULL, 0 );
    HRESULT hres = 0;
    IShellLink* psl;

    if (SUCCEEDED(hres))
    {

        // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
        // has already been called.
        hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (LPVOID*)&psl); //CLSCTX_ALL CLSCTX_INPROC_SERVER (void**)&psl (LPVOID*)&psl
        if (SUCCEEDED(hres))
        {
            IPersistFile* ppf;

            // Set the path to the shortcut target and add the description.
            psl->SetPath(lpszPathObj);
            psl->SetDescription(lpszDesc);
            psl->SetIconLocation(lpszPicture, 0);

            // Query IShellLink for the IPersistFile interface, used for saving the
            // shortcut in persistent storage.
            hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); //(void**)&psl (LPVOID*)&ppf

            if (SUCCEEDED(hres))
            {
                WCHAR wsz[MAX_PATH];

                // Save the link by calling IPersistFile::Save.

                hres = _wmakepath_s( wsz, _MAX_PATH, NULL, lpszPathLink,
                      L"MinGW-W64", L"lnk" );
                printf("\n120 : hres : %x\n", hres);
                hres = ppf->Save(wsz, TRUE); //TRUE
                printf("\n122 : hres : %x\n", hres);
                ppf->Release();
            }
            psl->Release();
        }
    }
    CoUninitialize();
    return hres;
}
void shortcut(const std::string& filePath, const std::string& picturePath, const std::string& description)
{
    //std::wstring wfile = std::wstring(file.begin(), file.end());
    //std::wstring wdescription = std::wstring(description.begin(), description.end());
    wchar_t* p = nullptr;
    //HRESULT hres = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &p);
    HRESULT hres = SHGetKnownFolderPath(FOLDERID_CommonPrograms, 0, NULL, &p);
    std::wstring location = p;
    //FOLDERID_CommonStartup //
    // FOLDERID_CommonPrograms//
    // FOLDERID_CommonStartMenu //

    //location+=L"\\mingw";

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string l = converter.to_bytes( location );

    std::string Filef = filePath, Picf = picturePath;
    std::replace(Filef.begin(), Filef.end(), '/', '\\');
    std::replace(Picf.begin(), Picf.end(), '/', '\\');
    //HRESULT CreateLink(LPCSTR lpszPathObj, LPCWSTR lpszPathLink, LPCSTR lpszDesc, LPCSTR lpszPicture);

    printf("\n\nWanna to create shortcut for:  %s,\n to:  %s,\n  picture: %s,\n description: %s\n\n",
           Filef.c_str(), l.c_str(), Picf.c_str(), description.c_str());

    CreateLink(Filef.c_str(), location.c_str(), description.c_str(), Picf.c_str());
}

#else

void shortcut(const std::string& file,  const std::string& icon, const std::string& description)
{
    printf("File to startup menu:  %s", file.c_str());
}

#endif