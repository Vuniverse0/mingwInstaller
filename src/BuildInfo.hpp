#ifndef MINGWINSTALLER_BUILDINFO_HPP
#define MINGWINSTALLER_BUILDINFO_HPP

#include "pages.hpp"


struct BuildInfo {
    std::string name;
    std::string version;
    std::string download;
    Arcs architecture;
    MgRs multithreading;
    std::size_t revision;
    ExcRs exception;
    Crt runtime;
};

struct SelectInfo {
    std::size_t version = 0;
    Arcs architecture =  Arcs::i686;
    MgRs multithreading = MgRs::win32;
    std::size_t revision = 0;
    ExcRs exception = ExcRs::error; ///TODO maybe fix it and make default sjlj
    Crt runtime = Crt::empty;
};

inline bool operator==(SelectInfo a, SelectInfo b)
{
    return a.version == b.version
    && a.revision == b.revision
    && a.architecture == b.architecture
    && a.multithreading == b.multithreading
    && a.runtime == b.runtime
    && a.exception == b.exception;
}


#endif //MINGWINSTALLER_BUILDINFO_HPP
