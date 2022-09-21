#include "Manager.hpp"

#include <filesystem>
#include <algorithm>

#include <curl/curl.h>

#include <Fl/Fl_PNG_Image.H>

#include "curl_tools.hpp"
#include "logo.h"
#include "link.h"

#ifndef NLOHMANN_JSON
#define FLAT_JSON
#endif

#ifdef TEST_FLATJSON
#define FLAT_JSON
#define NLOHMANN_JSON
#endif

#ifdef FLAT_JSON
#include "flatjson.hpp"
#endif

#ifdef NLOHMANN_JSON
#include <nlohmann/json.hpp>
#endif


Manager Manager::manager{};

extern "C" { int force_update(size_t, size_t); }

extern "C"{ int seven_z(int(*)(size_t, size_t), int numArgs, const char *args[]);}

enum class Page6state{download, downloading, extract, extracting, done, error };
void page6_set(Page6state state);

void progressSet(float);

void auto_extract();

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    if (TotalToDownload > 0.0)
        progressSet(static_cast<float>(NowDownloaded / TotalToDownload));
    return 0;
}

const std::vector<size_t>& Manager::getRevsForCandidate()
{
    revs.clear();
    for(auto& it: buffer){
        if(it.version == versions[downloadCandidate.version])
            revs.push_back(it.revision);
    }
    revs.erase(std::unique(revs.begin(), revs.end()), revs.end());
    std::sort(revs.begin(), revs.end(), std::greater()); ///[](auto a, auto b){return a > b;} );
    return revs;
}

const std::vector<std::string>& Manager::getVersions()
{
    if(versions.empty()) getInfo();
    return versions;
}

const std::vector<BuildInfo>& Manager::getInfo()
{
    if(buffer.empty()) {
        CURL *curl;
        CURLcode res;

        std::string readBuffer;
        curl = curl_easy_init();

        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/niXman/mingw-builds-binaries/releases");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Anon");

            for (auto &header: headers_strings)
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

        }

        fillBuffer(buffer, readBuffer); ///Parse
        /*
        for(const auto& buildInfo : buffer)
            if (std::find(versions.begin(), versions.end(), buildInfo.version) == versions.end())
                versions.push_back(buildInfo.version);
        */
        versions.resize(buffer.size());
        std::transform(buffer.cbegin(), buffer.cend(), versions.begin(), [](auto& buildInfo){return buildInfo.version;});
        versions.erase(std::unique(versions.begin(), versions.end()), versions.end());
        sortVersions();
    }
    return buffer;
}

BuildInfo Manager::parseName(const std::string& name)
{
    BuildInfo buildInfo;
    buildInfo.name = name;
    char buff [100] {};

    strcpy(buff, name.c_str());

    const char *tokenValue = strtok(buff, "-");

    for(size_t i = 0; tokenValue; ++i){
        switch (i) {
            case 0:
                buildInfo.architecture =
                        strcmp(tokenValue, "x86_64") == 0
                        ? Arcs::x86_64
                        : strcmp(tokenValue, "i686") == 0
                        ? Arcs::i686
                        : Arcs::error;
                break;
            case 1:
                buildInfo.version = tokenValue;
                break;
            case 3:
                buildInfo.multithreading =
                        strcmp(tokenValue, "win32") == 0
                        ? MgRs::win32
                        : strcmp(tokenValue, "posix") == 0
                        ? MgRs::posix
                        : MgRs::error;
                break;
            case 4:
                buildInfo.exception =
                        strcmp(tokenValue, "sjlj") == 0
                        ? ExcRs::sjlj
                        : strcmp(tokenValue, "dwarf") == 0
                        ? ExcRs::dwarf
                        : strcmp(tokenValue, "seh") == 0
                        ? ExcRs::seh
                        : ExcRs::error;
                break;
            case 6:
                buildInfo.revision = static_cast<size_t>(std::stoi(tokenValue+3));
                break;
            default: break;
        }
        tokenValue = strtok(nullptr, "-");
    }
    return buildInfo;
}

const BuildInfo &Manager::getCandidate()
{
    static const BuildInfo* last_build;
    static SelectInfo last_info {0};
    if(!last_info.version || !(last_info == downloadCandidate))
        last_info = downloadCandidate;
    else
        return *last_build;

    auto it = std::find_if(getInfo().begin(), getInfo().end(),
       [&](auto& member) -> bool {
            return
               member.version == versions[downloadCandidate.version]
            && member.revision == revs[downloadCandidate.revision]
            && member.architecture == downloadCandidate.architecture
            && member.multithreading == downloadCandidate.multithreading
            && member.exception == downloadCandidate.exception;
        });
    if(it == getInfo().end())
        throw std::runtime_error("Can't find build");
    last_build = &(*it);
    return *it;
}

void Manager::unpack()
{
    ///TODO remove all in folder of exist and remove if cancel
    std::string file = (std::filesystem::temp_directory_path() / getCandidate().name).string();
    const char* args[4]{"seven_z", "x", file.c_str()};
    std::filesystem::current_path(installDir);
    Fl::remove_timeout(Timer_CB);
    if(seven_z(force_update, 3, args))///!=0
        return ;
    status = Status::Done;
    Fl::repeat_timeout(1.0 / 60.0, Timer_CB);
}

void Manager::Timer_CB(void *userdata) {
    switch (manager.status) {
        case Status::Empty:
            break;
        case Status::Downloading:
            Manager::manager.downloading();
            break;
        case Status::Downloaded:
            Manager::manager.downloadEnd();
            break;
        case Status::Extracting:
            Manager::manager.unpack();
            break;
        case Status::Done:
            Manager::manager.extractEnd();
            //system("mklink /d shortcut_name Target_file"); ///Here need to create bat file
            break;
        case Status::Error:
            throw std::runtime_error("Error status in TimerCb");
    }
}

void Manager::download()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    http_handle = curl_easy_init();
    auto& candidate =  Manager::manager.getCandidate();

    ///"Accept: application/vnd.github+json" \ создаешь терминал с прописаным пасем в мингв
    /// распаковка в темп, распаковка куда выбор, Батник к бин заспускает в ней терминал
    ///и добавить батник в винду lzma

    if(!http_handle) throw std::runtime_error("Curl corrupted");

    curl_easy_setopt(http_handle, CURLOPT_URL, candidate.download.c_str());
    curl_easy_setopt(http_handle, CURLOPT_USERAGENT, "Anon");

    curl_easy_setopt(http_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(http_handle, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(http_handle, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(http_handle, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(http_handle, CURLOPT_TCP_KEEPALIVE, 1L);

    for(auto& header : Manager::manager.headers_strings)
        curl_easy_setopt(http_handle, CURLOPT_HEADERDATA, &header);

    curl_easy_setopt(http_handle, CURLOPT_HEADERFUNCTION, header_callback);

    curl_easy_setopt(http_handle, CURLOPT_PROGRESSFUNCTION, progress_func);
    curl_easy_setopt(http_handle, CURLOPT_NOPROGRESS, 0);
    /* open the file */
    std::string file = (std::filesystem::temp_directory_path() / getCandidate().name).string();
    dataFile = fopen(file.c_str(), "wb");

    if(dataFile) {
        curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, write_data);
        /* write the page body to this file handle */
        curl_easy_setopt(http_handle, CURLOPT_WRITEDATA,  (void*)dataFile);
    }else{
        throw std::runtime_error("Can't create download file");
    }

    multi_handle = curl_multi_init();/// init a multi stack

    curl_multi_add_handle(multi_handle, http_handle); ///add the individual transfers

    still_running = 1;

    status = Status::Downloading;

    page6_set(Page6state::downloading);

    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::extract()
{
    status = Status::Extracting;
    page6_set(Page6state::extracting);
    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::downloading()
{
    CURLMcode mc = curl_multi_perform(multi_handle, &(still_running));
    if (!mc) /* wait for activity, timeout or "nothing" */
        mc = curl_multi_poll(multi_handle, nullptr, 0, 10, 0);
    else{
        fprintf(stderr, "curl_multi_poll() failed, code %d.\n", (int) mc);
        status = Status::Error;
        downloadEnd();
        return;
    }
    if ( !still_running )
        status = Status::Downloaded;

    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::downloadEnd() {
    printf("Download Finished");
    fclose(dataFile);

    curl_multi_remove_handle(multi_handle, http_handle);
    curl_easy_cleanup(http_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    still_running = 0;

    multi_handle = http_handle = dataFile = nullptr;

    page6_set(Page6state::download);

    if(status == Status::Downloaded)
        auto_extract();
    else
        status = Status::Empty;

    ///TODO add cancel extract with rm of files
}

int Manager::extractCancel() const { return status != Status::Extracting; }

void Manager::extractEnd()
{
    if(status == Status::Extracting) {
        page6_set(Page6state::extract);
        status = Status::Downloaded;
    }else if(status == Status::Done) {
        page6_set(Page6state::done);
        createBat();
        link(installDir + "mingw32/mingw.bat", installDir+"Mingw");
    }else
        throw std::runtime_error("extractEnd: invalid status");
}

void Manager::cancel()
{
    if(status ==  Status::Downloaded)
        downloadEnd();
    else if(status == Status::Extracting)
        extractEnd();
    status = Status::Empty;
}

void Manager::fillBuffer(std::vector<BuildInfo> &buffer, const std::string& readBuffer)
{
#ifdef TEST_FLATJSON
    #undef FLAT_JSON
    #undef FLAT_JSON
    BuildInfo nl_buildInfo;
    nlohmann::json data = nlohmann::json::parse(readBuffer);
        for( auto& page : data )
            for (auto &link: page.at("assets")) {
                auto name = to_string(link.at("name"));
                nl_buildInfo.name = std::string(name.begin() + 1, name.end() - 1);
                nl_buildInfo = parseName(nl_buildInfo.name);
                nl_buildInfo.download = to_string(link.at("browser_download_url"));
                nl_buildInfo.download = std::string(nl_buildInfo.download.begin() + 1, nl_buildInfo.download.end() - 1);
                ///buffer.push_back( nl_buildInfo );
            }

    flatjson::fjson data{readBuffer.c_str(), readBuffer.c_str()+readBuffer.length()};
    assert(data.is_array());
    for(std::size_t size_d = data.size(), i = 0; i<size_d; ++i) {
        auto page_json = data.at(i).at("assets");
        assert(page_json.is_array());
        for(std::size_t size_p = page_json.size(), j = 0; j<size_p; ++j) {
            //assert(itp->is_object());
            auto link = page_json.at(j);
            BuildInfo buildInfo;
            buildInfo.name = link.at("name").to_string();///std::string(name.begin() + 1, name.end() - 1);
            buildInfo = parseName(buildInfo.name);
            buildInfo.download = link.at("browser_download_url").to_string();
            buffer.push_back(buildInfo);

            assert(buildInfo.name == nl_buildInfo.name);
            assert(buildInfo.revision == nl_buildInfo.revision);
            assert(buildInfo.architecture == nl_buildInfo.architecture);
            assert(buildInfo.exception == nl_buildInfo.exception);
            assert(buildInfo.multithreading == nl_buildInfo.multithreading);
            assert(buildInfo.version == nl_buildInfo.version);
            assert(buildInfo.download == nl_buildInfo.download);

        }
    }
#endif

#ifdef FLAT_JSON
    flatjson::fjson data{readBuffer.c_str(), readBuffer.c_str()+readBuffer.length()};
    assert(data.is_array());
    for(std::size_t size_d = data.size(), i = 0; i < size_d; ++i) {
        auto page_json = data.at(i).at("assets");
        assert(page_json.is_array());
        for(std::size_t size_p = page_json.size(), j = 0; j < size_p; ++j) {
            auto link = page_json.at(j);
            BuildInfo buildInfo;
            buildInfo.name = link.at("name").to_string();///std::string(name.begin() + 1, name.end() - 1);
            buildInfo = parseName(buildInfo.name);
            buildInfo.download = link.at("browser_download_url").to_string();

            buffer.push_back(buildInfo);
        }
    }
#endif

#ifdef NLOHMANN_JSON
    nlohmann::json data = nlohmann::json::parse(readBuffer);
        for( auto& page : data )
            for (auto &link: page.at("assets")) {
                auto name = to_string(link.at("name"));
                BuildInfo buildInfo;
                buildInfo.name = std::string(name.begin() + 1, name.end() - 1);
                buildInfo = parseName(buildInfo.name);
                buildInfo.download = to_string(link.at("browser_download_url"));
                buildInfo.download = std::string(buildInfo.download.begin() + 1, buildInfo.download.end() - 1);
                buffer.push_back( buildInfo );
            }
#endif
}

void Manager::sortVersions()
{
    std::sort(versions.begin(), versions.end(),
              [](auto& first, auto& second)->bool {
                  std::size_t fpos1{}, fpos2{};
                  std::size_t spos1{}, spos2{};
                  int f = stoi(first, &fpos1);
                  int s = stoi(second, &spos1);
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  f = stoi(std::string(first.begin() + 1 + fpos1, first.end()), &fpos2);
                  s = stoi(std::string(second.begin() + 1 + spos1, second.end()), &spos2);
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  f = stoi(std::string(first.begin() + 2 + fpos1 + fpos2, first.end()));
                  s = stoi(std::string(second.begin() + 2 + spos1 + spos2, second.end()));
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  return false;
                  ///TODO rewrite versions, make version class and normal comparing
              });
}

void Manager::logo()
{
    auto* logo = new Fl_PNG_Image("", logo_png, static_cast<int>(logo_png_len));
    auto* box_l = new Fl_Box( 0, 0, 312, 312);
    box_l->image(logo);
}

void Manager::createBat() const
{
    FILE *file = fopen("mingw32/mingw.bat", "wb");
    std::string bat = "@echo off\n"
                      "set PATH=" + installDir + "mingw32/bin/;%PATH%\n"
                      "%windir%\\system32\\cmd.exe";
    fwrite(bat.c_str(), 1, bat.size(), file);
    fclose(file);
}

