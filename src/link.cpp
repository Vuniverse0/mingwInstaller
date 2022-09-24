#include "link.h"
#include <filesystem>
#ifdef WIN32
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <sys/stat.h> // stat
#include <tchar.h> // _tcscpy,_tcscat,_tcscmp
#include <algorithm> // replace

using namespace std;

BOOL CreateShortcut(const char *ptchExecutableFileName, const char *ptchShortcutName, const char *icon)
{
    BOOL Res = TRUE;
    FILE *fp;

    if(NULL == (fp = fopen(ptchExecutableFileName, "r")))
    {
        printf("\nCan`t find executable file!\n");
        return FALSE;
    }
    fclose(fp);

    Res &= WritePrivateProfileString("InternetShortcut",
               "URL", ptchExecutableFileName, ptchShortcutName);
    Res &= WritePrivateProfileString("InternetShortcut",
               "IconIndex", "0", ptchShortcutName);
    Res &= WritePrivateProfileString("InternetShortcut",
               "IconFile", icon, ptchShortcutName);

    return Res;
}

void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName, const std::string& icon)
{
    CreateShortcut(ptchExecutableFileName.c_str(), ptchShortcutName.c_str(), icon.c_str());
}

double directory_delete(const char* pathname)
{
    string str(pathname);
    if (!str.empty())
    {
        while (*str.rbegin() == '\\' || *str.rbegin() == '/')
        {
            str.erase(str.size()-1);
        }
    }
    replace(str.begin(),str.end(),'/','\\');

    struct stat sb;
    if (stat((char *)str.c_str(),&sb) == 0 &&
        S_ISDIR(sb.st_mode))
    {
            HANDLE hFind;
            WIN32_FIND_DATA FindFileData;

            TCHAR DirPath[MAX_PATH];
            TCHAR FileName[MAX_PATH];

            _tcscpy(DirPath,(char *)str.c_str());
            _tcscat(DirPath,"\\*");
            _tcscpy(FileName,(char *)str.c_str());
            _tcscat(FileName,"\\");

            hFind = FindFirstFile(DirPath,&FindFileData);
            if (hFind == INVALID_HANDLE_VALUE) return 0;
            _tcscpy(DirPath,FileName);

            bool bSearch = true;
            while (bSearch)
            {
                if (FindNextFile(hFind,&FindFileData))
                {
                    if (!(_tcscmp(FindFileData.cFileName,".") &&
                        _tcscmp(FindFileData.cFileName,".."))) continue;
                    _tcscat(FileName,FindFileData.cFileName);
                    if ((FindFileData.dwFileAttributes &
                    FILE_ATTRIBUTE_DIRECTORY))
                    {
                        if (!directory_delete(FileName))
                        {
                            FindClose(hFind);
                            return 0;
                        }
                        RemoveDirectory(FileName);
                        _tcscpy(FileName,DirPath);
                    }
                    else
                    {
                        if (FindFileData.dwFileAttributes &
                            FILE_ATTRIBUTE_READONLY)
                            _chmod(FileName, _S_IWRITE);

                        if (!DeleteFile(FileName))
                        {
                            FindClose(hFind);
                            return 0;
                        }
                        _tcscpy(FileName,DirPath);
                    }
                }
                else
                {
                    if (GetLastError() == ERROR_NO_MORE_FILES)
                        bSearch = false;
                    else
                    {
                        FindClose(hFind);
                        return 0;
                    }
                }
            }
            FindClose(hFind);

            return (double)(RemoveDirectory((char *)str.c_str()) == true);
    }
    else
    {
        return 0;
    }
    }
    void showError(const char* error)
    {
         int msgboxID = MessageBox(
        NULL,
        "Check internet conection. Details?",
        "Problem",
        MB_YESNO
        );
        if(msgboxID == IDYES){
            MessageBox(
            NULL,
            error,
            "Problem",
            MB_OK
            );
        }
    }
}

#else
void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName, const std::string& icon)
{
    printf("Wanna to create shortcut for:  %s, named:  %s", ptchExecutableFileName.c_str(), ptchShortcutName.c_str());
}
double directory_delete(const char* pathname)
{
    std::filesystem::remove_all(pathname);
}
void showError(const char* error)
{
    printf("%s", error);
}
#endif