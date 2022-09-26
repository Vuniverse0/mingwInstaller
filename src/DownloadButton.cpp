#include "DownloadButton.hpp"

#include "Manager.hpp"

namespace DownloadCallbacks {
    static void download_stop_cb(Fl_Widget *button, void *);

    static void download_cb(Fl_Widget *button, void *)
    {
        button->label("Cancel");
        button->callback(download_stop_cb);
        Manager::manager.download();
    }

    static void download_stop_cb(Fl_Widget *button, void *)
    {
        button->label("Download");
        button->callback(download_cb);
        Manager::manager.downloadEnd();
    }

    static void extract_stop_cb(Fl_Widget *button, void *);

    static void extract_cb(Fl_Widget *button, void *) {
        button->label("Cancel");
        button->callback(extract_stop_cb);
        Manager::manager.extract();
    }

    static void extract_stop_cb(Fl_Widget *button, void *)
    {
        button->label("Extract");
        button->callback(extract_cb);
        Manager::manager.extractEnd();
    }
}

DownloadButton::DownloadButton(int x, int y, int w, int h, const char *label)
    : Fl_Button(x, y, w, h, label)
{
    Fl_Button::callback(DownloadCallbacks::download_cb);
}

void DownloadButton::extracting() { DownloadCallbacks::extract_cb(this, nullptr); }

void DownloadButton::downloading() { DownloadCallbacks::download_stop_cb(this, nullptr); }

void DownloadButton::download() { DownloadCallbacks::download_cb(this, nullptr); }
