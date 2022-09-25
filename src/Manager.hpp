#ifndef MINGWINSTALLER_MANAGER_HPP
#define MINGWINSTALLER_MANAGER_HPP

#include <array>

#include "BuildInfo.hpp"


enum class ExcRs;
enum class MgRs;
enum class Arcs;

class Manager final {
    explicit Manager() = default;
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


    static Fl_RGB_Image* logo(bool box = true);

    const std::vector<std::string>& getVersions();
    const std::vector<std::size_t>& getRevsForCandidate();
    void unpack();
    const std::array<const std::string, 2> headers_strings
    {
            "Authorization: Bearer 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",
            "Accept: application/vnd.github+json"
    };
private:
    std::array<std::string_view, 2> folderNames{"mingw32\0", "mingw64\0"};
    const BuildInfo& getCandidate();
    const std::vector<BuildInfo>& getInfo();
    static void fillBuffer(std::vector<BuildInfo>& buff, const std::string& data);
    void sortVersions();
    static BuildInfo parseName(const std::string& name);
    static void Timer_CB(void *userdata);

    std::vector<BuildInfo> buffer;
    std::vector<std::string> versions{};
    std::vector<std::size_t> revs{};

    void downloading();

    [[nodiscard]] std::string createBat();
    [[nodiscard]] std::string createIcon() ;

    void *http_handle = nullptr, //CURL
    *multi_handle = nullptr; //CURLM
    FILE *dataFile = nullptr;
    int still_running = 0;

    enum class Status{
        Empty,
        Downloading,
        Downloaded,
        Extracting,
        Done,
        Error
    } status = Status::Empty;
};


#endif //MINGWINSTALLER_MANAGER_HPP
