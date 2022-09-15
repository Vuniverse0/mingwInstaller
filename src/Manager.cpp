#include "Manager.hpp"

#include <filesystem>

#include <curl/curl.h>
#include <nlohmann/json.hpp> ///TODO remove json
///TODO move download code from download button to manager

Manager Manager::manager{};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t header_callback(char *buffer, size_t size,
                              size_t nitems, void *userdata)
{
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    return nitems * size;
}

const std::vector<size_t>& Manager::getRevsForCandidate()
{
    auto it = buffer.begin(), end = buffer.end() ;
    revs.clear();
    do{
        it = std::find_if(it, end, [&](auto& member) -> bool {
            return member.version == versions[downloadCandidate.version];
        });
        revs.push_back(it->revision);
    }while( it++ != end );
    std::sort(revs.begin(), revs.end(),
              [](auto a, auto b){return a > b;} );
    return revs;
}

const std::vector<std::string>& Manager::getVersions()
{
    if ( versions.empty() )
        throw std::runtime_error("Please call Manager::getInfo before");
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
                if(std::find(versions.begin(), versions.end(), buildInfo.version) == versions.end())
                    versions.push_back(buildInfo.version);
            }
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
        tokenValue = strtok(NULL, "-");
    }
    return buildInfo;
}

const BuildInfo &Manager::getCandidate()
{
    static BuildInfo* last_build;
    static SelectInfo last_info {0};
    if(!last_info.version || !(last_info == downloadCandidate))
        last_info = downloadCandidate;
    else
        return *last_build;

    auto it = std::find_if(buffer.begin(), buffer.end(),
       [&](auto& member) -> bool {

            return
               member.version == versions[downloadCandidate.version]
            && member.revision == revs[downloadCandidate.revision]
            && member.architecture == downloadCandidate.architecture
            && member.multithreading == downloadCandidate.multithreading
            && member.exception == downloadCandidate.exception;
        });
    if(it == buffer.end())
        throw std::runtime_error("Can't find build");
    last_build = &(*it);
    return *it;
}

std::string Manager::tempPath(const std::string& str)
{
    return (std::filesystem::temp_directory_path() / str).string();
}

extern "C"{ int seven_z(int numArgs, const char *args[]);}

//#include "LzmaEnc.h"
//#include "LzmaDec.h"


int Manager::unpack(const std::string &arc) const
{
    /// target:: installDir
    ///source:: arc

    ///) LzmaDec.h + LzmaDec.c + Types.h
    const char* args[4]{"./7zdec", "x", arc.c_str()};
    std::filesystem::current_path(installDir);
    seven_z(3, args);

    //lzma_auto_decoder()
    return 0;
}
