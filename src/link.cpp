#include "link.h"

#ifdef WIN32
///include "stdafx.h"
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

/*
  -------------------------------------------------------------------
  Description:
    Creates the actual 'lnk' file (assumes COM has been initialized).

  Parameters:
    pszTargetfile    - File name of the link's target.
    pszTargetargs    - Command line arguments passed to link's target.
    pszLinkfile      - File name of the actual link file being created.
    pszDescription   - Description of the linked item.
    iShowmode        - ShowWindow() constant for the link's target.
    pszCurdir        - Working directory of the active link.
    pszIconfile      - File name of the icon file used for the link.
    iIconindex       - Index of the icon in the icon file.

  Returns:
    HRESULT value >= 0 for success, < 0 for failure.
  --------------------------------------------------------------------
  */
  static HRESULT CreateShortCut(LPSTR pszTargetfile, LPSTR pszTargetargs,
                                LPSTR pszLinkfile, LPSTR pszDescription,
                                LPSTR pszIconfile,
                                int iShowmode = SW_SHOWNORMAL, //LPSTR pszCurdir,
                                 int iIconindex = 0)
  {
    HRESULT       hRes;                  /* Returned COM result code */
    IShellLink*   pShellLink;            /* IShellLink object pointer */
    IPersistFile* pPersistFile;          /* IPersistFile object pointer */
    WORD          wszLinkfile[MAX_PATH]; /* pszLinkfile as Unicode
                                            string */
    int           iWideCharsWritten;     /* Number of wide characters
                                            written */

    hRes = E_INVALIDARG;
    if (
         (pszTargetfile != NULL) && (strlen(pszTargetfile) > 0) &&
         (pszTargetargs != NULL) &&
         (pszLinkfile != NULL) && (strlen(pszLinkfile) > 0) &&
         (pszDescription != NULL) &&
         (iShowmode >= 0) &&
         //(pszCurdir != NULL) &&
         (pszIconfile != NULL) &&
         (iIconindex >= 0)
       )
    {
      hRes = CoCreateInstance(
        &CLSID_ShellLink,     /* pre-defined CLSID of the IShellLink
                                 object */
        NULL,                 /* pointer to parent interface if part of
                                 aggregate */
        CLSCTX_INPROC_SERVER, /* caller and called code are in same
                                 process */
        &IID_IShellLink,      /* pre-defined interface of the
                                 IShellLink object */
        &pShellLink);         /* Returns a pointer to the IShellLink
                                 object */
      if (SUCCEEDED(hRes))
      {
        /* Set the fields in the IShellLink object */
        hRes = pShellLink->lpVtbl->SetPath(pShellLink,
                                             pszTargetfile);
        hRes = pShellLink->lpVtbl->SetArguments(pShellLink,
                                              pszTargetargs);
        if (strlen(pszDescription) > 0)
        {
          hRes = pShellLink->lpVtbl->SetDescription(pShellLink,
                                                  pszDescription);
        }
        if (iShowmode > 0)
        {
          hRes = pShellLink->lpVtbl->SetShowCmd(pShellLink,
                                                       iShowmode);
        }
        //if (strlen(pszCurdir) > 0)
        {
          //hRes = pShellLink->lpVtbl->SetWorkingDirectory(pShellLink,
            //                                              pszCurdir);
        }
        if (strlen(pszIconfile) > 0 && iIconindex >= 0)
        {
          hRes = pShellLink->lpVtbl->SetIconLocation(pShellLink,
                                            pszIconfile, iIconindex);
        }

        /* Use the IPersistFile object to save the shell link */
        hRes = pShellLink->lpVtbl->QueryInterface(
          pShellLink,                /* existing IShellLink object */
          &IID_IPersistFile,         /* pre-defined interface of the
                                        IPersistFile object */
          &pPersistFile);            /* returns a pointer to the
                                        IPersistFile object */
        if (SUCCEEDED(hRes))
        {
          iWideCharsWritten = MultiByteToWideChar(CP_ACP, 0,
                                               pszLinkfile, -1,
                                               wszLinkfile, MAX_PATH);
          hRes = pPersistFile->lpVtbl->Save(pPersistFile,
                                                   wszLinkfile, TRUE);
          pPersistFile->lpVtbl->Release(pPersistFile);
        }
        pShellLink->lpVtbl->Release(pShellLink);
      }

    }
    return (hRes);
  }

void link(const std::string& pszTargetfile, const std::string& pszLinkfile
          const std::string& pszDescription, const std::string& pszIconfile)
{
    //std::wstring stemp1 = std::wstring(lpszPathObj.begin(), lpszPathObj.end());
    //std::wstring stemp2 = std::wstring(lpszPathLink.begin(), lpszPathLink.end());
    //std::wstring stemp3 = std::wstring(lpszDesc.begin(), lpszDesc.end());
    //CreateLink(stemp1.c_str(), lpszPathLink.c_str(), stemp3.c_str());
    CreateLink(pszTargetfile.c_str(), "\0", pszDescription.c_str(), pszIconfile.c_str());
    //printf("    lpszPathObj: %s", lpszPathObj.c_str());
    //printf("    lpszPathLink: %s", lpszPathLink.c_str());
    //printf("    lpszDesc: %s", lpszDesc.c_str());
}
#else
void link(const std::string& lpszPathObj, const std::string& lpszPathLink, const std::string& lpszDesc)
{

}
#endif