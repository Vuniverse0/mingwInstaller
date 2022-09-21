#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>

#include "pages.hpp"
#include "Manager.hpp"


namespace {
    Fl_Window *G_win = 0;
    Fl_Wizard *G_wiz = 0;
}

void back_cb(Fl_Widget*,void*) { G_wiz->prev(); }
void next_cb(Fl_Widget*,void*) { G_wiz->next(); } ///while(1){force_update(0);  }} for tests
void done_cb(Fl_Widget*w,void*) { w->window()->hide(); }

///welcome page  TODO

void page_();  ///hello

void page_0();  ///choose dir

void page_1();  ///version

void page_2();  ///architecture

void page_3();  ///multithreading realization (mrt)

void page_4(); ///exceptions realization

void page_5();  ///revision

void page_6();  ///download

void progressSet(float rate);

extern "C" {
int force_update(size_t now, size_t all) {
    Fl::wait(1.0/60.);
    progressSet(static_cast<float>(now) / static_cast<float>(all));
    return Manager::manager.extractCancel();
}
}

int main(int argc, char **argv) {
    Fl::scheme("gtk+");

    //G_win = new Fl_Window(400, 300, width, height, "Mingw Installer");
    G_win = new Fl_Window(400, 300, width, height, "Mingw Installer");
    G_wiz = new Fl_Wizard(0, 0, width, height);
    G_win->icon(Manager::logo(false));

    page_();
    page_1();
    page_2();
    page_3();
    page_4();
    page_5();
    page_0();
    page_6();

    G_wiz->end();
    G_win->end();
    G_win->show(argc, argv);

    return Fl::run();
}
