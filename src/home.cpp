#include "home.hpp"

#ifdef WIN32
#include <shlobj.h>

#include <stdexcept>


std::string home()
{
    LPSTR path[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))){
        std::string res {path};
        res+="/";
        std::replace(res.begin(), res.end(), '\\', '/');
        return res;
    }
    throw std::runtime_error("cant get home path");
}

#else

std::string home()
{
    //Hardcoded for debug on Linux
    return "/home/vuniverse";
}

#endif
