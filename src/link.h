#ifndef MINGWINSTALLER_LINK_H
#define MINGWINSTALLER_LINK_H

#include <string>

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
void link(const std::string& lpszPathObj, const std::string& lpszPathLink, const std::string& lpszDesc);

#endif //MINGWINSTALLER_LINK_H
