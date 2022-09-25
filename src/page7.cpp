#include "pages.hpp"

#include "Manager.hpp"
#include "DownloadButton.hpp"

#include <Fl/Fl_Group.H>
#include <Fl/Fl_Progress.H>
#include <Fl/Fl_Box.H>


///download
namespace {
    Fl_Progress *progress = nullptr;
    Fl_Box *out = nullptr;
    DownloadButton *download = nullptr;
    Fl_Button *back = nullptr;
}

enum class Page6state{download, downloading, extract, extracting, done, error };

void auto_extract(){if(download) download->extracting(); else throw std::runtime_error("download page 6");}

void page7_set(Page6state state)
{
    if(  !out   ) throw std::runtime_error("out not defined page 6");
    if(!progress) throw std::runtime_error("progress not defined page 6");
    if(!download) throw std::runtime_error("download not defined page 6");

    switch (state) {
        case Page6state::download:
            out->label("Download              ");
            progress->hide();
            progress->value(0.f);
            break;
        case Page6state::downloading:
            out->label("Downloading...       ");
            progress->show();
            break;
        case Page6state::extract:
            out->label("Extract              ");
            progress->hide();
            progress->value(0.f);
            break;
        case Page6state::extracting:
            out->label("Extracting...       ");
            progress->show();
            break;
        case Page6state::done:
            out->label("Done              ");
            download->callback(done_cb);
            download->label("Finish");
            if(back) back->hide();
            else throw std::runtime_error("back is null error page6");
            break;
        case Page6state::error:
            throw std::runtime_error("state::error page6");
    }
}

void progressSet(float rate)
{
    if(progress) progress->value(rate);
    else throw std::runtime_error("progress not defined page 6");
}

static void reset(Fl_Widget *button, void * d)
{
    Manager::manager.cancel();
    if(download) download->downloading();
    page7_set(Page6state::download);
    back_cb(button, d);
}

void page_7() {
    auto *g = new Fl_Group(0, 0, width, height);

    download = new DownloadButton(button_x + png_size, button_y, button_width, button_height, "Download");

    back = new Fl_Button(button_x - button_width - 20 + png_size, button_y, button_width, button_height, "@<- Back");
    back->callback(reset);

    progress = new Fl_Progress(40 + png_size, 200, 500, 25);
    progress->maximum(1);
    progress->minimum(0);
    progress->hide();
    progress->value(0.f);

    out = new Fl_Box(20 + png_size, 100, 25, 25, "Download");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    Manager::logo();

    g->end();
}