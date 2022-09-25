#include "pages.hpp"

#include "Manager.hpp"

#include "Fl/Fl_File_Chooser.H"
#include "Fl/Fl_Multiline_Output.H"

///directory
namespace {
    Fl_File_Chooser*  fc;
    Fl_Button *next;
    Fl_Multiline_Output *box;

    void choice_callback(Fl_File_Chooser *obj, void *)
    {
        if(fc->visible()) return;
        Manager::manager.installDir = obj->value();
        box->value(Manager::manager.installDir.c_str());
        box->show();
        if(obj != fc)
            throw std::runtime_error("Otval choice_callback page0(5.5) ");
        delete fc;
        fc = nullptr;
        next->show();
    }

    void call_back_page_1(Fl_Widget* w, void*) {
        if (!fc) {
            fc = new Fl_File_Chooser(nullptr, nullptr, 4, nullptr);
            fc->callback(choice_callback);
            fc->show();
        }else if(!fc->visible())
            fc->show();
        //if(fc->visible()) w->callback(next_cb) ; else fc->show();
    }
}


void page_0()
{
    Fl_Group *g = new Fl_Group(0, 0, width, height);

    next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);
    next->hide();
    Fl_Button *back = new Fl_Button(button_x-button_width-20+png_size, button_y, button_width, button_height, "@<- Back");
    back->callback(back_cb);

    Fl_Button *set_dirr = new Fl_Button((180+290)/2+png_size, 150, 100, 25, "Chose");
    set_dirr->callback(call_back_page_1);

    box = new Fl_Multiline_Output(100+png_size, 200, 500, 25,"Install in: ");
    box->hide();

    Fl_Box *out = new Fl_Box(20+png_size, 100, 500, 25, "Select an install directory");
    out->labelsize(45);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    Manager::logo();

    g->end();
}
