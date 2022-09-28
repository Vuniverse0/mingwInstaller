#include <string>
#include <algorithm>
#include <filesystem>

#ifdef WIN32

#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "shobjidl.h"
#include "shellapi.h"
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
                   LPCSTR lpszDesc, LPCSTR lpszPicture, LPCWSTR name)
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
                      name, L"lnk" );

                using convert_type = std::codecvt_utf8<wchar_t>;
                std::wstring_convert<convert_type, wchar_t> converter;
                std::string l = converter.to_bytes( wsz );

                //printf("\n : hres : %x\n, wsz: %s\n", hres, l.c_str());

                hres = ppf->Save(wsz, TRUE);

                //printf("\n : hres : %x\n, wsz: %s\n", hres, l.c_str());
                ppf->Release();
            }
            psl->Release();
        }
    }
    CoUninitialize();
    return hres;
}

//destination:
// 0 - FOLDERID_Programs
// 1 - FOLDERID_Desktop
void shortcut(std::string filePath, std::string picturePath, const std::string& name, std::size_t destination)
{
    wchar_t* p = nullptr;

    if(destination == 0)
        HRESULT hres = SHGetKnownFolderPath(FOLDERID_Programs, 0, NULL, &p);
    else if(destination == 1)
        HRESULT hres = SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &p);

    std::wstring location = p;

    std::wstring n{name.begin(), name.end()};

    std::replace(picturePath.begin(), picturePath.end(), '/', '\\');
    std::replace(filePath.begin(), filePath.end(), '/', '\\');

    printf("\n\nWanna to create shortcut for:  %s,\n to:  %s,\n  picture: %s,\n\n",
           filePath.c_str(), location.c_str(), picturePath.c_str());

    CreateLink(filePath.c_str(), location.c_str(), "Compiler", picturePath.c_str(), n.c_str());
}

std::size_t directory_delete(std::string pathname)
{
    pathname += "\0\0";
    SHFILEOPSTRUCTA file{
        NULL,
        FO_DELETE,
        pathname.c_str(),
        NULL,
        FOF_NO_UI | FOF_NOCONFIRMATION,
        FALSE,
        NULL,
        NULL
        };
    return SHFileOperationA(&file);
}

void showError(const char* error)
{
    printf("%s", error);

    int msgboxID = MessageBox(NULL, "Check internet conection. Details?", "Problem", MB_YESNO);

    if(msgboxID == IDYES){
        MessageBox(
        NULL,
        error,
        "Problem",
        MB_OK
        );
    }
}

#else

//destination:
// 0 - FOLDERID_Programs
// 1 - FOLDERID_Desktop
void shortcut(std::string filePath, std::string picturePath, const std::string& name, std::size_t destination)
{

    printf("\n\nWanna to create shortcut for:  %s,\n to:  ?,\n  picture: %sn\n",
           filePath.c_str(), picturePath.c_str());
}

std::size_t directory_delete(std::string pathname)
{
    return std::filesystem::remove_all(pathname);
}

void showError(const char* error)
{
    printf("%s", error);
}
#endif