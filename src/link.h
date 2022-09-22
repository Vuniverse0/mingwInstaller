#ifndef MINGWINSTALLER_LINK_H
#define MINGWINSTALLER_LINK_H

#include <string>

void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName, const std::string& icon);
double directory_delete(const char* pathname);
#endif //MINGWINSTALLER_LINK_H
