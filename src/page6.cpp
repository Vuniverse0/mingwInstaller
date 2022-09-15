#include <Fl/Fl_Progress.H>

#include "DownloadButton.hpp"


///download
namespace {
    Fl_Button *next_button = nullptr;
    Fl_Progress *progress = nullptr;
}


void doneDownload()
{
    if ( next_button ) next_button->show();
    else throw std::runtime_error("Next button not defined page 7");
}

void progres(double rate)
{
    progress->value(static_cast<float>(rate));
}

void page_6() {
    auto *g = new Fl_Group(0, 0, 400, 300);

    Fl_Button *download = new DownloadButton(290, 265, 100, 25, "Download");
    progress = new Fl_Progress((290+290)/2 - 250 , 200, 500, 25);
    progress->maximum(1);
    progress->minimum(0);

    Fl_Button *next = new DownloadButton(290, 265, 100, 25, "Next @->");
    next_button = next;
    next->callback(next_cb);
    next->hide();

    auto *back = new Fl_Button(180, 265, 100, 25, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20, 100, 25, 25, "Download");

    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    g->end();
}
