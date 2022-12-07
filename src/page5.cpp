#include "pages.hpp"

#include "Manager.hpp"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>


void update_6();

///exceptions realization
namespace {
    const std::array<std::string_view, 3> exceptions{
        "sjlj\0",
        "dwarf\0",
        "seh\0"
    };

    bool isDwarf = true;
    Fl_Choice *choice;

    void choice_callback(Fl_Button*, void*)
    {
        Manager::manager.downloadCandidate.exception =
            (static_cast<std::size_t>(choice->value()) != 0
            ? isDwarf
            ? ExcRs::dwarf
            : ExcRs::seh
            : ExcRs::sjlj);
        update_6();
    }
}

void update_5()
{
    Manager::manager.downloadCandidate.exception = ExcRs::sjlj;

    auto res_page2 = static_cast<std::uint8_t>(Manager::manager.downloadCandidate.architecture);
    isDwarf = !res_page2;

    if(!Manager::manager.getSjlj()){
        Manager::manager.downloadCandidate.exception = isDwarf ? ExcRs::dwarf : ExcRs::seh;
        next_cb(nullptr, nullptr);
        update_6();
        return;
    }

    choice->clear();
    choice->add(exceptions[0].data());
    choice->add(exceptions[res_page2 + 1].data());
    choice->value(0);
    update_6();
}

void page_5()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    auto *back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                               button_width, button_height, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20 + png_size, 100, 25, 25, "Select an exceptions");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200 + png_size, 150, static_cast<int>(15 * exceptions[0].size() * 2), 45);

    choice->add(exceptions[0].data());
    choice->add(exceptions[1].data());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
