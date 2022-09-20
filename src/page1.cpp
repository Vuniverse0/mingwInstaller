#include "pages.hpp"
#include "Manager.hpp"
#include "Fl/Fl_File_Chooser.H"

void update5(); ///revision

///version
namespace {
    Fl_Choice *choice;
    void choice_callback(Fl_Button *obj, void *)
    {
        Manager::manager.downloadCandidate.version = static_cast<size_t>(choice->value());
        update5();
    }
}


void page_1()
{
    Fl_Group *g = new Fl_Group(0, 0, width, height);

    Fl_Button *next = new Fl_Button(290, 265, 100, 25, "Next @->");
    next->callback(next_cb);

    Fl_Button *back = new Fl_Button(180, 265, 100, 25, "@<- Back");
    back->callback(back_cb);

    Fl_Box *out = new Fl_Box(20, 100, 25, 25, "   Select a version");
    out->labelsize(50);
    out->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    auto* logo = new Fl_PNG_Image("", logo_png, static_cast<int>(logo_png_len));
    auto* box = new Fl_Box( 0, 0, 50, 50);
    box->image(logo);

    const std::vector<std::string>& versions =  Manager::manager.getVersions();

    choice = new Fl_Choice(200, 150, 15 * (12), 45);

    for(auto &version : versions)
        choice->add(version.c_str());

    choice->value(0);
    choice->textsize(20);
    choice->callback((Fl_Callback *) choice_callback);

    g->end();
}
