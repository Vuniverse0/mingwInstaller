#ifndef MINGWINSTALLER_MANAGER_HPP
#define MINGWINSTALLER_MANAGER_HPP

#include "BuildInfo.hpp"

#include <array>
#include <vector>
#include <stdexcept>


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
    [[nodiscard]] int extractCancel() const;
    void cancel();

    //Fl_RGB_Image*
    static void* logo(bool box = true);

    const std::vector<std::string>& getVersions();
    const std::vector<std::size_t>& getRevsForCandidate();

private:
    std::array<std::string_view, 2> folderNames{"mingw32\0", "mingw64\0"};
    const std::array<const std::string, 2> headers_strings
    {
            "Authorization: Bearer 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",
            "Accept: application/vnd.github+json"
    };

    const BuildInfo& getCandidate();
    const std::vector<BuildInfo>& getInfo();
    static void fillBuffer(std::vector<BuildInfo>& buff, const std::string& data);
    static BuildInfo parseName(const std::string& name);
    static void Timer_CB(void *userdata);

    void sortVersions();
    void unpack();
    void downloading();

    [[nodiscard]] std::string createBat();
    [[nodiscard]] std::string createIcon() ;

    std::vector<BuildInfo> buffer;
    std::vector<std::string> versions{};
    std::vector<std::size_t> revs{};

    //CURL
    void *http_handle = nullptr,
    //CURLM
    *multi_handle = nullptr;

    int still_running = 0;

    FILE *dataFile = nullptr;

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
