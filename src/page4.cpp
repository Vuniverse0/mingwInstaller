#include "pages.hpp"
#include "Manager.hpp"


///exceptions realization
namespace {
    const std::array<std::string_view, 3> exrs{
            "sjlj\0",
            "dwarf\0",
            "seh\0"
    };
    bool isDwarf;
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *)
    {
        Manager::manager.downloadCandidate.exception =
                (
                static_cast<size_t>(choice->value()) != 0
                ? isDwarf
                ? ExcRs::dwarf
                : ExcRs::seh
                : ExcRs::sjlj
                );
    }
}


void update4()
{
    auto res_page2 = static_cast<int8_t>(Manager::manager.downloadCandidate.architecture);
    choice->clear();
    choice->add(exrs[0].data());
    choice->add(exrs[res_page2 + 1].data());
    isDwarf = !res_page2;
    choice->value(0);
    Manager::manager.downloadCandidate.exception = ExcRs::sjlj;
}

void page_4() {
    Fl_Group *g = new Fl_Group(0, 0, 400, 300);
    Fl_Button *next = new Fl_Button(290, 265, 100, 25, "Next @->");
    next->callback(next_cb);
    Fl_Button *back = new Fl_Button(180, 265, 100, 25, "@<- Back");
    back->callback(back_cb);

    Fl_Box *out = new Fl_Box(20, 100, 25, 25, "Select a exceptions");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200, 150, 15 * (exrs[0].size()*2), 45); //,"Select a version");

    choice->add(exrs[0].data());
    choice->add(exrs[1].data());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);
    g->end();

}
