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

void showError(const char* error);

void page_about(); ///about

void page_0();  ///hello

void page_1();  ///version

void page_2();  ///architecture

void page_3();  ///multithreading realization (mrt)

void page_4(); ///exceptions realization

void page_5();  ///revision

void page_6();  ///choose dir

void page_7();  ///download

int main(int argc, char **argv) try {
    Fl::scheme("gtk+");

    //G_win = new Fl_Window(400, 300, width, height, "Mingw Installer");
    G_win = new Fl_Window(400, 300, width, height, "Mingw Installer");
    G_wiz = new Fl_Wizard(0, 0, width, height);
    G_win->icon(Manager::logo(false));

    page_about();
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
    next_cb(G_wiz, nullptr);
    return Fl::run();
} catch ( std::exception& error ){
    showError(error.what());
}
