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
void next_cb(Fl_Widget*,void*) { G_wiz->next(); }
void done_cb(Fl_Widget*w,void*) { w->window()->hide(); }

///welcome page  TODO

void page_();  ///version

void page_0();  ///version

void page_1();  ///version

void page_2();  ///architecture

void page_3();  ///multithreading realization (mrt)

void page_4(); ///exceptions realization

void page_5();  ///revision

void page_6();  ///download

void page_7();  ///finish

int main(int argc, char **argv) {
    Fl::scheme("gtk+");

    G_win = new Fl_Window(800, 600, width, height, "Example Wizard");
    G_wiz = new Fl_Wizard(0, 0, width, height);
    Manager::manager.getInfo();

    page_();
    page_0();
    page_1();
    page_2();
    page_3();
    page_4();
    page_5();
    page_6();
    page_7();

    G_wiz->end();
    G_win->end();
    G_win->show(argc, argv);

    return Fl::run();
}
