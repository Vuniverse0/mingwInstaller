#ifndef MINGWINSTALLER_MANAGER_HPP
#define MINGWINSTALLER_MANAGER_HPP

#include <array>

#include "BuildInfo.hpp"


enum class ExcRs;
enum class MgRs;
enum class Arcs;

class Manager final {
    Manager() = default;
    static BuildInfo parseName(const std::string& name);
public:
    static Manager manager;

    SelectInfo downloadCandidate{};
    std::string installDir{"/tmp"};
    const BuildInfo& getCandidate();
    const std::vector<BuildInfo>& getInfo();
    const std::vector<std::string>& getVersions();
    const std::vector<size_t>& getRevsForCandidate();
    int unpack(const std::string& arc) const;
    const std::array<const std::string, 2> headers_strings
    {
            "Authorization: Bearer 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",
            "Accept: application/vnd.github+json"
    };
    static std::string tempPath(const std::string& str);
private:
    std::vector<BuildInfo> buffer;
    std::vector<std::string> versions{};
    std::vector<size_t> revs{};
};


#endif //MINGWINSTALLER_MANAGER_HPP
