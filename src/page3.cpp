#include "pages.hpp"
#include "Manager.hpp"


///multithreading realization (mrt)
namespace {
    const std::array<std::string_view, 2> mtrs{
            "win32\0",
            "posix\0"
    };
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *)
    {
        size_t value = choice->value();
        Manager::manager.downloadCandidate.multithreading = !value ? MgRs::win32 : MgRs::posix;
    }
}


void page_3()
{
    Fl_Group *g = new Fl_Group(0,0,width, height);
    Fl_Button *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->"); next->callback(next_cb);
    Fl_Button *back = new Fl_Button(button_x-button_width-20+png_size, button_y, button_width, button_height,"@<- Back"); back->callback(back_cb);

    Fl_Box *out = new Fl_Box(20+png_size,100,25,25,"Select a thread model");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

    choice = new Fl_Choice(200+png_size,150,15*(mtrs[0].size() * 2),45); //,"Select a version");

    for ( auto& mtr : mtrs )
        choice->add(mtr.data());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback*) choice_callback);

    Manager::logo();

    g->end();

}
