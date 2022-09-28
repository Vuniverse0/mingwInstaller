#ifndef MINGWINSTALLER_LINK_HPP
#define MINGWINSTALLER_LINK_HPP

#include <string>


void link(const std::string& ExecutableFileName, const std::string& ShortcutName,
          const std::string& icon);
double directory_delete(const char* pathname); //TODO  SHFileOperationA
void showError(const char* error);

#endif //MINGWINSTALLER_LINK_HPP
