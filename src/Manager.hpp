#ifndef MINGWINSTALLER_MANAGER_HPP
#define MINGWINSTALLER_MANAGER_HPP

#include <array>

#include "BuildInfo.hpp"


enum class ExcRs;
enum class MgRs;
enum class Arcs;

class Manager final {
    Manager() = default;
public:
    static Manager manager;

    SelectInfo downloadCandidate{};///TODO set candidate?
    std::string installDir{};///TODO set install dir
    void download();
    void downloadEnd();
    void extract();
    void extractEnd();
    int extractCancel() const;
    void cancel();

    static void logo();

    const std::vector<std::string>& getVersions();
    const std::vector<size_t>& getRevsForCandidate();
    void unpack();
    const std::array<const std::string, 2> headers_strings
    {
            "Authorization: Bearer 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",
            "Accept: application/vnd.github+json"
    };
private:
    const BuildInfo& getCandidate();
    const std::vector<BuildInfo>& getInfo();
    static void fillBuffer(std::vector<BuildInfo>& buff, const std::string& data);
    void sortVersions();
    static BuildInfo parseName(const std::string& name);
    static void Timer_CB(void *userdata);

    std::vector<BuildInfo> buffer;
    std::vector<std::string> versions{};
    std::vector<size_t> revs{};

    void downloading();

    void *http_handle = 0; //CURL
    void *multi_handle = 0; //CURLM
    FILE *dataFile = 0;
    int still_running = 0;

    enum class Status{
        Empty,
        Downloading,
        Downloaded,
        Extracting,
        Done,
        Error
    } status;
};


#endif //MINGWINSTALLER_MANAGER_HPP
