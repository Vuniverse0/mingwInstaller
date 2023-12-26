#include "pages.hpp"

#include "Manager.hpp"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>


///exceptions realization
namespace {
    const std::array<std::string_view, 2> runtimes{
            "msvcrt\0",
            "ucrt\0",
    };

    Fl_Choice *choice;

    void choice_callback(Fl_Button*, void*)
    {
        Manager::manager.downloadCandidate.runtime =
            (static_cast<std::size_t>(choice->value()) == 0
            ? Crt::msvcrt
            : Crt::ucrt);
    }
}


void update_6()
{
    if(Manager::manager.getMcf()){
        Manager::manager.downloadCandidate.runtime = Crt::ucrt;
        choice->clear();
        choice->add(runtimes[1].data());
        choice->value(0);
        return;
    }else{
        choice->clear();
        for(auto &runtime : runtimes)
            choice->add(runtime.data());
        choice->value(0);
        Manager::manager.downloadCandidate.runtime = Crt::msvcrt;
    }
    
    if(!Manager::manager.getCrt()){
        Manager::manager.downloadCandidate.runtime = Crt::empty;
        return;
    }
    
    Manager::manager.downloadCandidate.runtime = Crt::msvcrt;
    choice->value(0);
}

static void update_cb(Fl_Widget*, void*)
{
    back_cb(nullptr, nullptr);
    if(!Manager::manager.getSjlj())
        back_cb(nullptr, nullptr);
}

void page_6()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    auto *back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                               button_width, button_height, "@<- Back");
    back->callback(update_cb);

    auto *out = new Fl_Box(20 + png_size, 100, 25, 25, "Select a C runtime");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200 + png_size, 150, static_cast<int>(15 * runtimes[0].size() * 2), 45);

    for(auto &runtime : runtimes)
        choice->add(runtime.data());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
