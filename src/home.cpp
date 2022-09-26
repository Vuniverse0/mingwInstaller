#include "home.hpp"

#ifdef WIN32
#include <shlobj.h>

#include <stdexcept>


std::string home()
{
    LPSTR path = new char[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))){
        std::string res {path};
        delete path;
        return res;
    }else{
        delete path;
        throw std::runtime_error("cant get home path");
    }
}

#else

std::string home()
{
    //Hardcoded for debug on Linux
    return "/home/vuniverse/";
}

#endif
