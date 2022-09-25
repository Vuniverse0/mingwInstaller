#ifndef MINGWINSTALLER_BUILDINFO_HPP
#define MINGWINSTALLER_BUILDINFO_HPP

#include <string>

enum class ExcRs{ sjlj, dwarf, seh, error};
enum class MgRs{ win32, posix, error};
enum class Arcs{ i686, x86_64, error};

struct BuildInfo {
    std::string name;
    std::string version;
    std::string download;
    Arcs architecture;
    MgRs multithreading;
    ExcRs exception;
    std::size_t revision;
};

struct SelectInfo {
    std::size_t version = 0;
    Arcs architecture =  Arcs::i686;
    MgRs multithreading = MgRs::win32;
    ExcRs exception = ExcRs::sjlj;
    std::size_t revision = -1;
};

inline bool operator==(const SelectInfo& a, const SelectInfo& b)
{
    return
       a.version == b.version
    && a.revision == b.revision
    && a.architecture == b.architecture
    && a.multithreading == b.multithreading
    && a.exception == b.exception
    ;
}

#endif //MINGWINSTALLER_BUILDINFO_HPP
