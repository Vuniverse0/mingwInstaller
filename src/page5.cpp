#include "Manager.hpp"


///revision
namespace {
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *) {
        Manager::manager.downloadCandidate.revision = static_cast<size_t>( choice->value() );
    }
}


void update5()
{
    const std::vector<size_t>& revs = Manager::manager.getRevsForCandidate();
    choice->clear();
    for ( auto rev : revs )
        choice->add( ( std::string("rev") + std::to_string(rev) ).c_str() );
    Manager::manager.downloadCandidate.revision = 0;
    choice->value(0);
}

void page_5() {
    Fl_Group *g = new Fl_Group(0, 0, width, height);
    //Fl_Button *done = new Fl_Button(290, 265, 100, 25, "Finish");
    //done->callback(done_cb);
    Fl_Button *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    Fl_Button *back = new Fl_Button(button_x-button_width-20+png_size, button_y, button_width, button_height, "@<- Back");
    back->callback(back_cb);

    Fl_Box *out = new Fl_Box(20+png_size, 100, 25, 25, "Select a build revision");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200+png_size, 150, 15 * 8, 45); //,"Select a version");

    update5();
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
