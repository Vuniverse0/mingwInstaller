#include "pages.hpp"


///hello page


void page_()
{
    Fl_Group *g = new Fl_Group(0, 0, width, height);

    Fl_Button *next = new Fl_Button(290, 265, 100, 25, "Next @->");
    next->callback(next_cb);

    Fl_Button *back = new Fl_Button(180, 265, 100, 25, "Exit");
    back->callback(done_cb);

    Fl_Box *out = new Fl_Box(20, 100, 25, 25, "Hello");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    g->end();
}
