#include "Manager.hpp"

#include <filesystem>
#include <algorithm>

#include <curl/curl.h>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <Fl/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>

#include "curl_tools.hpp"
#include "icon.hpp"
#include "logo.hpp"

#include "flatjson.hpp"


void progressSet(float rate);

extern "C" {
    int force_update(size_t now, size_t all)
    {
        Fl::wait(1.0/60.);
        progressSet(static_cast<float>(now) / static_cast<float>(all));
        return Manager::manager.extractCancel();
    }

    int seven_z(int(*)(size_t, size_t), int numArgs, const char *args[]);
}

enum class Page7state{download, downloading, extract, extracting, done, error };
void page7_set(Page7state state);

void auto_extract();

void shortcut(std::string filePath, std::string picturePath, const std::string& name, std::size_t destination);

std::size_t directory_delete(std::string pathname);

//Curl progress
int progress_func(void*, double TotalToDownload, double NowDownloaded, double, double)
{
    if(TotalToDownload > 0.0)
        progressSet(static_cast<float>(NowDownloaded / TotalToDownload));
    return 0;
}

const std::vector<std::size_t>& Manager::getRevsForCandidate()
{
    revs.clear();
    for(auto &it : buffer){
        if(it.version == versions[downloadCandidate.version])
            revs.push_back(it.revision);
    }
    revs.erase(std::unique(revs.begin(), revs.end()), revs.end());
    std::sort(revs.begin(), revs.end(), std::greater());
    return revs;
}

const std::vector<std::string>& Manager::getVersions()
{
    if(versions.empty())
        getInfo();
    return versions;
}

const std::vector<BuildInfo>& Manager::getInfo()
{
    if(buffer.empty()){
        CURL *curl;
        CURLcode res;

        std::string readBuffer;
        curl = curl_easy_init();

        if(curl){
	        curl_easy_setopt(curl , CURLOPT_VERBOSE, 1L);
	        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/niXman/mingw-builds-binaries/releases");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Anon");
	        char error[CURL_ERROR_SIZE]; /* needs to be at least this big */ //a
	        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);//a
            for (auto &header: headers_strings)
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
	        if(res)
                showError(error);
        }else {
            showError("Curl corrupted: init error");
            throw std::runtime_error("curl in init error");
        }
	    if(readBuffer.empty()){
            showError("Curl error");
            throw std::runtime_error("curl after init error");
        }

        fillBuffer(buffer, readBuffer); ///Parse

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

    for(std::size_t i = 0; tokenValue; ++i){
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
			: strcmp(tokenValue, "mcf") == 0
			? MgRs::mcf
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
            case 5:
                if(strncmp(tokenValue, "rt", 2) != 0) {
                    //std::cout<<"TokenValue "<<tokenValue<<std::endl;
                    buildInfo.runtime =
                            strcmp(tokenValue, "msvcrt") == 0
                            ? Crt::msvcrt
                            : strcmp(tokenValue, "ucrt") == 0
                            ? Crt::ucrt
                            : Crt::error;
                    break;
                }else{
                    buildInfo.runtime = Crt::empty;
                    i++;
                }
            case 6:
                break;
            case 7:
                //std::cout<<"TokenValue "<<tokenValue<<std::endl;
                buildInfo.revision = static_cast<std::size_t>(std::stoi(tokenValue+3));
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
            && member.exception == downloadCandidate.exception
            && member.runtime == downloadCandidate.runtime;
        });
    if(it == getInfo().end()){
#ifndef NDEBUG
        for(auto &item : getInfo())
            std::cerr<<item<<std::endl;
        std::cerr<<"\n\n\n\n"<<downloadCandidate<<std::endl;
#endif
        showError("Logical error\n");
        throw std::runtime_error("Can't find build");
    }
    last_build = &(*it);
    return *it;
}

void Manager::unpack()
{
    Fl::remove_timeout(Timer_CB);

    std::string file = (std::filesystem::temp_directory_path() / getCandidate().name).string();

    const char* args[4]{"seven_z", "x", file.c_str()};

    std::filesystem::current_path(installDir);

    std::filesystem::path mingw(std::filesystem::current_path()/
    folderNames[downloadCandidate.architecture == Arcs::x86_64].data());

    if(exists(mingw))
        directory_delete(folderNames[downloadCandidate.architecture == Arcs::x86_64].data());

    auto res = seven_z(force_update, 3, args);

    printf("\nFile %s\n", file.c_str());
    printf("Dir %s\n", installDir.c_str());
    printf("Cur %s\n", std::filesystem::current_path().string().c_str());

    if(res){
        if(exists(mingw))
            directory_delete(folderNames[downloadCandidate.architecture == Arcs::x86_64].data());
        if(res == 1488)
            showError("try set another folder or run installer again, access denied(maybe)");
        showError("Unpack error, try run installer again");
        return ;
    }

    status = Status::Done;

    Fl::repeat_timeout(1.0 / 60.0, Timer_CB);
}

void Manager::Timer_CB(void *)
{
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
            break;
        case Status::Error:
            showError("Logical error: state failed");
            throw std::runtime_error("Error status in TimerCb");
    }
}

void Manager::download()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    http_handle = curl_easy_init();
    auto& candidate =  Manager::manager.getCandidate();

    if(!http_handle) {
        showError("Curl corrupted");
        throw std::runtime_error("Curl corrupted");
    }

    curl_easy_setopt(http_handle, CURLOPT_SSL_VERIFYPEER, false);
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
        showError("file error");
        throw std::runtime_error("Can't create download file");
    }

    multi_handle = curl_multi_init();/// init a multi stack

    curl_multi_add_handle(multi_handle, http_handle); ///add the individual transfers

    still_running = 1;

    status = Status::Downloading;

    page7_set(Page7state::downloading);

    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::extract()
{
    status = Status::Extracting;
    page7_set(Page7state::extracting);
    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::downloading()
{
    CURLMcode mc = curl_multi_perform(multi_handle, &(still_running));
    if (!mc){ /* wait for activity, timeout or "nothing" */
        mc = curl_multi_poll(multi_handle, nullptr, 0, 10, nullptr);
        if(mc) showError("download error");
    }else{
        fprintf(stderr, "curl_multi_poll() failed, code %d.\n", (int) mc);
        status = Status::Error;
        showError("download error");
        downloadEnd();
        return;
    }
    if ( !still_running )
        status = Status::Downloaded;

    Fl::repeat_timeout(1.0/60.0, Timer_CB);
}

void Manager::downloadEnd()
{
    if (status == Status::Downloading || status == Status::Downloaded) {
        fclose(dataFile);

        curl_multi_remove_handle(multi_handle, http_handle);
        curl_easy_cleanup(http_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();

        still_running = 0;

        multi_handle = http_handle = dataFile = nullptr;

        page7_set(Page7state::download);
    }
    if(status == Status::Downloaded){
        auto_extract();
        printf("Download Finished");
    }else{
        status = Status::Empty;
        printf("Download Canceled");
    }
}

int Manager::extractCancel() const { return status != Status::Extracting; }

void Manager::extractEnd()
{
    if(status == Status::Extracting) {
        page7_set(Page7state::extract);
        status = Status::Downloaded;
    }else if(status == Status::Done) {
        page7_set(Page7state::done);
        shortcut(installDir + createBat(),
                 installDir + createIcon(),
                 (downloadCandidate.architecture == Arcs::i686 ? "MinGW-W64-32bit" : "MinGW-W64-64bit"),
                 0);
        if(desktopShortcut)
            shortcut(installDir + createBat(),
                     installDir + createIcon(),
                     (downloadCandidate.architecture == Arcs::i686 ? "MinGW-W64-32bit" : "MinGW-W64-64bit"),
                     1);
    }else{
        showError("extractEnd: invalid status");
        throw std::runtime_error("extractEnd: invalid status");
    }
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
    flatjson::fjson data{readBuffer.c_str(), readBuffer.c_str()+readBuffer.length()};
    assert(data.is_array());
    for(std::size_t size_d = data.size(), i = 0; i < size_d; ++i) {
        auto page_json = data.at(i).at("assets");
        assert(page_json.is_array());
        for(std::size_t size_p = page_json.size(), j = 0; j < size_p; ++j) {
            auto link = page_json.at(j);
            BuildInfo buildInfo;
            buildInfo.name = link.at("name").to_string();
            buildInfo = parseName(buildInfo.name);
            buildInfo.download = link.at("browser_download_url").to_string();
            buffer.push_back(buildInfo);
        }
    }
}

void Manager::sortVersions()
{
    std::sort(versions.begin(), versions.end(),
              [](auto& first, auto& second)->bool {
                  std::size_t fpos1{}, fpos2{};
                  std::size_t pos1{}, pos2{};
                  int f = stoi(first, &fpos1);
                  int s = stoi(second, &pos1);
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  f = stoi(std::string(first.begin() + 1 + fpos1, first.end()), &fpos2);
                  s = stoi(std::string(second.begin() + 1 + pos1, second.end()), &pos2);
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  f = stoi(std::string(first.begin() + 2 + fpos1 + fpos2, first.end()));
                  s = stoi(std::string(second.begin() + 2 + pos1 + pos2, second.end()));
                  if( f > s )
                      return true;
                  else if( f < s )
                      return false;
                  return false;
                  ///TODO rewrite versions, make version class and normal comparing
              });
}

Fl_RGB_Image* Manager::logo(bool box)
{
    auto* logo = new Fl_PNG_Image("", getLogo().pointer, static_cast<int>(getLogo().size));
    if(box) {
        auto *box_l = new Fl_Box(0, 0, 312, 312);
        box_l->image(logo);
    }
    return logo;
}

std::string Manager::createIcon()
{
    std::string file_name = folderNames[downloadCandidate.architecture == Arcs::x86_64].data();
    file_name += "/icon.ico";
    FILE *file = fopen(file_name.c_str(), "wb");
    fwrite(getIcon().pointer, 1, getIcon().size, file);
    fclose(file);
    return file_name;
}

std::string Manager::createBat()
{
    std::string file_name = folderNames[downloadCandidate.architecture == Arcs::x86_64].data();
    file_name += "/mingw.bat";
    FILE *file = fopen(file_name.c_str(), "wb");
    std::string bat = "@echo off\n"
                      "set PATH=" + Manager::manager.installDir +
                      folderNames[downloadCandidate.architecture == Arcs::x86_64].data() + "/bin/;%PATH%\n"
                      "%windir%\\system32\\cmd.exe";
    fwrite(bat.c_str(), 1, bat.size(), file);
    fclose(file);
    return file_name;
}

Manager Manager::manager{};

bool Manager::getSjlj()
{
    auto it = std::find_if(buffer.begin(), buffer.end(), [&](auto &member){
        return member.version == versions[downloadCandidate.version]
        && member.revision == revs[downloadCandidate.revision]
        && member.architecture == downloadCandidate.architecture
        && member.multithreading == downloadCandidate.multithreading
        && member.exception == ExcRs::sjlj
    ;});

    return it != buffer.end();
}

bool Manager::getCrt()
{
    auto it = std::find_if(buffer.begin(), buffer.end(), [&](auto &member){
        return member.version == versions[downloadCandidate.version]
        && member.revision == revs[downloadCandidate.revision]
        && member.architecture == downloadCandidate.architecture
        && member.multithreading == downloadCandidate.multithreading
        && member.exception == downloadCandidate.exception
        && member.runtime != Crt::empty
    ;});

    return it != buffer.end();
}

bool Manager::getMcf()//TODO rename to are and are rename to get
{
    return (downloadCandidate.multithreading == MgRs::mcf);
}
bool Manager::areMcf()
{
    auto it = std::find_if(buffer.begin(), buffer.end(), [&](auto &member){
        return  member.multithreading == MgRs::mcf
    ;});

    return it != buffer.end();
}
