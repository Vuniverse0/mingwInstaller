#include "link.h"

#ifdef WIN32
#include <windows.h>
#include <stdio.h>

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

#else
void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName, const std::string& icon)
{
    printf("Wanna to create shortcut for:  %s, named:  %s", ptchExecutableFileName.c_str(), ptchShortcutName.c_str());
}
#endif