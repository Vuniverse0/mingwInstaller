#include "pages.hpp"

#include "Manager.hpp"
#include "DownloadButton.hpp"

#include <Fl/Fl_Progress.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>


///download
namespace {
    Fl_Progress *progress = nullptr;
    Fl_Box *out = nullptr;
    DownloadButton *download = nullptr;
    Fl_Button *back = nullptr;
}


void auto_extract()
{
    if(download)
        download->extracting();
    else
        throw std::runtime_error("download page 6");
}

enum class Page7state{download, downloading, extract, extracting, done, error};

void page7_set(Page7state state)
{
    if(  !out   ) throw std::runtime_error("out not defined page 6");
    if(!progress) throw std::runtime_error("progress not defined page 6");
    if(!download) throw std::runtime_error("download not defined page 6");

    switch (state) {
        case Page7state::download:
            out->label("Download              ");
            progress->hide();
            progress->value(0.f);
            break;
        case Page7state::downloading:
            out->label("Downloading...       ");
            progress->show();
            break;
        case Page7state::extract:
            out->label("Extract              ");
            progress->hide();
            progress->value(0.f);
            break;
        case Page7state::extracting:
            out->label("Extracting...       ");
            progress->show();
            break;
        case Page7state::done:
            out->label("Done              ");
            download->callback(done_cb);
            download->label("Finish");
            if(back) back->hide();
            else throw std::runtime_error("back not defined page 6");
            break;
        case Page7state::error:
            throw std::runtime_error("state::error page6");
    }
}

void progressSet(float rate)
{
    if(progress) progress->value(rate);
    else throw std::runtime_error("progress not defined page 6");
}

static void reset(Fl_Widget *button, void *data)
{
    Manager::manager.cancel();
    if(download) download->downloading();
    page7_set(Page7state::download);
    back_cb(button, data);
}

void page_6()
{
    auto *g = new Fl_Group(0, 0, width, height);

    download = new DownloadButton(button_x + png_size, button_y, button_width, button_height, "Download");

    back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                         button_width, button_height, "@<- Back");
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
