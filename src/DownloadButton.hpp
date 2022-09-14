#ifndef MINGWINSTALLER_DOWNLOADBUTTON_HPP
#define MINGWINSTALLER_DOWNLOADBUTTON_HPP

#include <curl/curl.h>

#include "pages.hpp"


static void download_cb(Fl_Widget*, void*);

struct DownloadButton : Fl_Button {
    DownloadButton(int x, int y, int w, int h, const char *label = 0);

    static void Timer_CB(void *userdata);
    void startDownloading();
    void endDownloading();

private:
    CURL *http_handle = 0;
    CURLM *multi_handle = 0;
    FILE *dataFile = 0;

    int still_running = 1;

    enum class DownloadStatus
    {
        Empty,
        Process,
        Done,
        Error
    } isDone = DownloadStatus::Empty;
};


#endif //MINGWINSTALLER_DOWNLOADBUTTON_HPP
