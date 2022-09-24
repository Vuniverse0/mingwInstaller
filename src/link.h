#ifndef MINGWINSTALLER_LINK_H
#define MINGWINSTALLER_LINK_H

#include <string>

void link(const std::string& ptchExecutableFileName, const std::string& ptchShortcutName, const std::string& icon);
double directory_delete(const char* pathname);
void showError(const char* error);

#endif //MINGWINSTALLER_LINK_H
