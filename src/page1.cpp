#include "pages.hpp"
#include "Manager.hpp"
#include "Fl/Fl_File_Chooser.H"


void update5(); ///revision

///version
namespace {
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *)
    {
        Manager::manager.downloadCandidate.version = static_cast<std::size_t>(choice->value());
        update5();
    }
}

void page_1()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x + png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    auto *back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                               button_width, button_height, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20 + png_size, 100, 25, 25, "   Select a version");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    const std::vector<std::string>& versions =  Manager::manager.getVersions();

    choice = new Fl_Choice(200 + png_size, 150, 15 * 12, 45);

    for(auto &version : versions)
        choice->add(version.c_str());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
