#include "link.h"

#ifdef WIN32
#include <windows.h>
#include <stdio.h>

BOOL CreateShortcut(const char *ptchExecutableFileName, const char *ptchShortcutName)
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
               "IconFile", ptchExecutableFileName, ptchShortcutName);

    return Res;
}

void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName)
{
    CreateShortcut(ptchExecutableFileName.c_str(), ptchShortcutName.c_str());
}

#else
void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName)
{

}
#endif