#include "link.h"

#ifdef WIN32
///include "stdafx.h"
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"


static HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc) {
    HRESULT hres;
    IShellLink *psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID * ) & psl);
    if (SUCCEEDED(hres)) {
        IPersistFile *ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID * ) & ppf);

        if (SUCCEEDED(hres)) {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}

void link(const std::string& lpszPathObj, const std::string& lpszPathLink, const std::string& lpszDesc)
{
    //std::wstring stemp1 = std::wstring(lpszPathObj.begin(), lpszPathObj.end());
    //std::wstring stemp2 = std::wstring(lpszPathLink.begin(), lpszPathLink.end());
    //std::wstring stemp3 = std::wstring(lpszDesc.begin(), lpszDesc.end());
    //CreateLink(stemp1.c_str(), stemp2.c_str(), stemp3.c_str());
    CreateLink(lpszPathObj.c_str(), lpszPathLink.c_str(), lpszDesc.c_str());
}
#else
void link(const std::string& lpszPathObj, const std::string& lpszPathLink, const std::string& lpszDesc)
{
}
#endif