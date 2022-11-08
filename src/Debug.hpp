#ifndef MINGWINSTALLER_DEBUG_HPP
#define MINGWINSTALLER_DEBUG_HPP

#include <iostream>

#include "BuildInfo.hpp"

#define DEBUG_BUILD_AND_SELECT_INFO


struct Manager;

template<class Manager_T = Manager>
std::ostream& operator<<(std::ostream& out, const SelectInfo& selectInfo)
{
    out << "\nVersion : " << Manager_T::manager.versions[selectInfo.version]
        << "\narchitecture : " <<
        (selectInfo.architecture == Arcs::i686 ? "i686" : (selectInfo.architecture == Arcs::x86_64 ? "x86_64" : "error"))
        << "\nmultithreading : " <<
        (selectInfo.multithreading == MgRs::win32 ? "win32" : (selectInfo.multithreading == MgRs::posix ? "posix" : "error"))
        << "\nexception : " << (selectInfo.exception == ExcRs::sjlj ? "sjlj" :
        (selectInfo.exception == ExcRs::dwarf ? "dwarf" : (selectInfo.exception == ExcRs::seh ? "seh" : "error")))
        << "\nrevision : " << Manager_T::manager.revs[selectInfo.revision] << '\n';
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const BuildInfo& buildInfo)
{
    out << "\nName : " << buildInfo.name
    << "\nVersion : " << buildInfo.version
    << "\nDownload : " << buildInfo.download
    << "\narchitecture : " <<
    (buildInfo.architecture == Arcs::i686 ? "i686" : (buildInfo.architecture == Arcs::x86_64 ? "x86_64" : "error"))
    << "\nmultithreading : " <<
    (buildInfo.multithreading == MgRs::win32 ? "win32" : (buildInfo.multithreading == MgRs::posix ? "posix" : "error"))
    << "\nexception : " << (buildInfo.exception == ExcRs::sjlj ? "sjlj" :
    (buildInfo.exception == ExcRs::dwarf ? "dwarf" : (buildInfo.exception == ExcRs::seh ? "seh" : "error")))
    << "\nrevision : " << buildInfo.revision << '\n';
    return out;
}

#endif //MINGWINSTALLER_DEBUG_HPP
