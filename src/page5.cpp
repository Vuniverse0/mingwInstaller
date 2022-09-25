#include "pages.hpp"

#include "Manager.hpp"

#include <Fl/Fl_File_Chooser.H>


///revision
namespace {
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *)
    {
        Manager::manager.downloadCandidate.revision = static_cast<std::size_t>(choice->value());
    }
}

void update5()
{
    const std::vector<std::size_t>& revs = Manager::manager.getRevsForCandidate();
    choice->clear();
    for(const auto& rev : revs) {
        char rev_buff[10] = "rev";
        choice->add(strcat(rev_buff, std::to_string(rev).c_str()));
    }
    Manager::manager.downloadCandidate.revision = 0;
    choice->value(0);
}

void page_5()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto *next = new Fl_Button(button_x + png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);

    auto *back = new Fl_Button(button_x - button_width - 20 + png_size, button_y,
                               button_width, button_height, "@<- Back");
    back->callback(back_cb);

    auto *out = new Fl_Box(20 + png_size, 100, 25, 25, "Select a build revision");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    choice = new Fl_Choice(200 + png_size, 150, 15 * 8, 45);

    update5();
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    Manager::logo();

    g->end();
}
