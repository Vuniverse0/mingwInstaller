#include "pages.hpp"

#include "Manager.hpp"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>


///revision
namespace {
    Fl_Choice *choice;
    void choice_callback(Fl_Button *, void *) {
        Manager::manager.downloadCandidate.revision = static_cast<std::size_t>( choice->value() );
    }
}


void update4()
{
    const std::vector<std::size_t>& revs = Manager::manager.getRevsForCandidate();
    choice->clear();

    for ( auto rev : revs )
        choice->add( ( std::string("rev") + std::to_string(rev) ).c_str() );

    Manager::manager.downloadCandidate.revision = 0;
    choice->value(0);
}

void update_5();

static void update_cb(Fl_Widget*, void*)
{
    next_cb(nullptr, nullptr);
    update_5();
}

void page_4()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(update_cb);

    auto *back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                               button_width, button_height, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20 + png_size, 100, 25, 25, "Select a build revision");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200 + png_size, 150, 15 * 8, 45);

    update4();
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
