#include "DownloadButton.hpp"

#include <cmath>

#include "Manager.hpp"

#include "Fl/Fl_File_Chooser.H"


void progres(double);

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded,
                  double TotalToUpload, double NowUploaded)
{
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }

    double fractiondownloaded = NowDownloaded / TotalToDownload;
    progres(fractiondownloaded);

#ifdef CONSOLE
    int totaldotz= 40;

    int dotz = (int) round(fractiondownloaded * totaldotz);

    int ii=0;
    printf("%3.0f%% [",fractiondownloaded*100);
    for ( ; ii < dotz;ii++) {
        printf("=");
    }
    for ( ; ii < totaldotz;ii++) {
        printf(" ");
    }
    printf("]\r");
    fflush(stdout);
#endif
    return 0;
}

static void download_cb(Fl_Widget* button, void*)
{
    ((DownloadButton*)button)->startDownloading();
    button->hide();
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

static size_t header_callback(char *buffer, size_t size,
                              size_t nitems, void *userdata)
{
    /* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
    /* 'userdata' is set with CURLOPT_HEADERDATA */
    return nitems * size;
}

DownloadButton::DownloadButton(int x, int y, int w, int h, const char *label)
    : Fl_Button(x, y, w, h, label)
{
    Fl_Button::callback(download_cb);
}

void DownloadButton::Timer_CB(void *userdata)
{
    auto *pb = (DownloadButton*)userdata;

    if ( pb->isDone == DownloadStatus::Process ) {

        CURLMcode mc = curl_multi_perform(pb->multi_handle, &(pb->still_running));
        if (!mc)
            /* wait for activity, timeout or "nothing" */
            mc = curl_multi_poll(pb->multi_handle, NULL, 0, 10, 0);
        if ( !pb->still_running ) {
            if ( pb->isDone == DownloadStatus::Process )
                pb->isDone = DownloadStatus::Done;
            pb->endDownloading();
            return;
        }else{
            Fl::repeat_timeout(1.0/60.0, Timer_CB, userdata);
        }

        if ( mc ) {
            fprintf(stderr, "curl_multi_poll() failed, code %d.\n", (int) mc);
            pb->isDone = DownloadStatus::Error;
            pb->endDownloading();
            return;
        }
        ///Fl::repeat_timeout(1.0/60.0, Timer_CB, userdata);
    }
}
#include<iostream>
void DownloadButton::startDownloading()
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
    dataFile = fopen(Manager::tempPath(candidate.name).c_str(), "wb");

    if(dataFile) {
        curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, write_data);
        /* write the page body to this file handle */
        curl_easy_setopt(http_handle, CURLOPT_WRITEDATA,  (void*)dataFile);
    }else{
        throw std::runtime_error("Can't create download file");
    }

    /* init a multi stack */
    multi_handle = curl_multi_init();
///TODO remake choose page
/// TODO write CURLOPT_PROGRESSFUNCTION for GUI
    /* add the individual transfers */
    curl_multi_add_handle(multi_handle, http_handle);

    Fl::add_timeout(1.0/60.0, Timer_CB, (void*)this);       // 24fps timer

    isDone = DownloadStatus::Process;
}

void DownloadButton::endDownloading()
{
    fclose(dataFile);

    curl_multi_remove_handle(multi_handle, http_handle);
    curl_easy_cleanup(http_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    multi_handle = http_handle = dataFile = nullptr;

    if(isDone == DownloadStatus::Done) {
        Manager::manager.unpack(Manager::tempPath(Manager::manager.getCandidate().name));
        doneDownload();
    }else{
        still_running = 0;
        isDone = DownloadStatus::Empty;
    }
}
