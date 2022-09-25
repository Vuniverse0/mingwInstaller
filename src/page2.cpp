#include "pages.hpp"
#include "Manager.hpp"


#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>


///exceptions realization
void update4();

///architecture
namespace {
    const std::array<std::string_view, 2> archs{
        "32 bit\0",
        "64 bit\0"
    };

    Fl_Choice *choice;

    void choice_callback(Fl_Button *, void *) {
        Manager::manager.downloadCandidate.architecture = choice->value() ?  Arcs::x86_64 : Arcs::i686;
        update4();
    }
}


void page_2() {
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    auto *back = new Fl_Button(button_x-button_width-20+png_size, button_y, button_width, button_height, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20+png_size, 100, 25, 25, "Select an architecture");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200 + png_size, 150, static_cast<int>(15 * archs[0].size() * 2), 45);

    for (auto &arch: archs)
        choice->add(arch.data());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
