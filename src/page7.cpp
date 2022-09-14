#include "pages.hpp"

///finish
namespace {

}


void page_7() {
    Fl_Group *g = new Fl_Group(0, 0, width, height);

    Fl_Button *done = new Fl_Button(290, 265, 100, 25, "Finish");
    done->callback(done_cb);;

    Fl_Box *out = new Fl_Box(20, 100, 25, 25, "Finish");

    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    g->end();
}