#ifndef MINGWINSTALLER_MANAGER_HPP
#define MINGWINSTALLER_MANAGER_HPP

#include <array>

#include <FL/Fl_RGB_Image.H>

#include "BuildInfo.hpp"

#ifndef NDEBUG
#include "Debug.hpp"
#endif


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

    static Fl_RGB_Image* logo(bool box = true);

    const std::vector<std::string>& getVersions();
    const std::vector<std::size_t>& getRevsForCandidate();
    bool getSjlj();
    bool getCrt();
    bool desktopShortcut = false;

private:
#ifdef DEBUG_BUILD_AND_SELECT_INFO
    template<class Manager_T>
    friend std::ostream& operator<<(std::ostream& out, const SelectInfo& selectInfo);
#endif

    const std::array<const std::string, 2> headers_strings{
            "Authorization: Bearer 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",
            "Accept: application/vnd.github+json"
    };

    std::array<std::string_view, 2> folderNames{
        "mingw32\0",
        "mingw64\0"
    };

    const BuildInfo& getCandidate();
    const std::vector<BuildInfo>& getInfo();
    static void fillBuffer(std::vector<BuildInfo>& buff, const std::string& data);
    void sortVersions();
    static BuildInfo parseName(const std::string& name);
    static void Timer_CB(void *userdata);

    void unpack();
    void downloading();

    [[nodiscard]] std::string createBat();
    [[nodiscard]] std::string createIcon();

    //CURL
    void *http_handle = nullptr;
    //CURLM
    void *multi_handle = nullptr;

    int still_running = 0;

    FILE *dataFile = nullptr;

    std::vector<BuildInfo> buffer;
    std::vector<std::string> versions{};
    std::vector<std::size_t> revs{};

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
