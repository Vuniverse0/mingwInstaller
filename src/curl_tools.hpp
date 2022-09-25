#ifndef MINGWINSTALLER_CURL_TOOLS_HPP
#define MINGWINSTALLER_CURL_TOOLS_HPP

#include <string>

static size_t write_data(void *ptr, std::size_t size, std::size_t nmemb, void *stream)
{
    std::size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

static std::size_t WriteCallback(void *contents, std::size_t size, std::size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static std::size_t header_callback(char *buffer, std::size_t size, std::size_t nitems, void *userdata)
{
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    return nitems * size;
}

#endif //MINGWINSTALLER_CURL_TOOLS_HPP
