#include <string>
#include <locale>
#include <codecvt>

#ifdef WIN32
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include "shobjidl.h"
#include <cwchar>

std::wstring getProgramsFolder();

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
// lpszPath     - Working directory of target Obj file // Mingw?
// lpszDesc     - Address of a buffer that contains a description of the
//                Shell link, stored in the Comment field of the link
//                properties.

    HRESULT                     CreateLink(
    LPCSTR                      lpszPathObj,
    LPCSTR                      lpszPathLink,
    LPCSTR                      lpszPath,
    LPCSTR                      lpszDesc  = "Description") //TOTO HERE !!! ADAPTATE

/*============================================================================*/
{
    IShellLink* psl = NULL;
    HRESULT hres = CoInitialize(NULL);

    //if (!SUCCEEDED(hres))
      //  C_ASSERT(FALSE);

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.

    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);
        psl->SetWorkingDirectory(lpszPath);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            //if (!SUCCEEDED(hres))
              //  C_ASSERT(FALSE);

            ppf->Release();
        }
        psl->Release();
    }

    CoUninitialize();

    return hres;
}
void shortcut(const std::string& file,  const std::string& workPath, const std::string& description)
{
    //std::wstring wfile = std::wstring(file.begin(), file.end());
    //std::wstring wdescription = std::wstring(description.begin(), description.end());
    std::wstring location = getProgramsFolder();
    location+=L"/MinGW-W64.lnk";

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string l = converter.to_bytes( location );

    CreateLink(file.c_str(), l.c_str(), workPath.c_str(), description.c_str());
}

#else

void shortcut(const std::string& file,  const std::string& workPath, const std::string& description)
{
    printf("File to startup menu:  %s", file.c_str());
}

#endif