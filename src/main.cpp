#include "version.hpp"

#include "pages.hpp"

#include "Manager.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Wizard.H>


namespace {
    Fl_Window *G_win = nullptr;
    Fl_Wizard *G_wiz = nullptr;
}


void back_cb(Fl_Widget*,void*) { if(G_wiz) G_wiz->prev(); }
void next_cb(Fl_Widget*,void*) { if(G_wiz) G_wiz->next(); }
void done_cb(Fl_Widget*w,void*) { w->window()->hide(); }

///about
void page_about();

///hello
void page_0();

///version
void page_1();

///architecture
void page_2();

///multithreading realization (mrt)
void page_3();

///revision
void page_4();

///exceptions realization
void page_5();

///C runtime
void page_6();

///choose dir
void page_7();

///download
void page_8();

int main(int argc, char **argv) try {
    if(argc > 1 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "v"))){
        printf("version: %s", MINGW_W64_INSTALLER_VERSION);
        return 0;
    }

    Fl::scheme("gtk+");

    G_win = new Fl_Window(400, 300, width, height, "MinGW-W64 Installer");
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
    page_8();

    G_wiz->end();
    G_win->end();
    G_win->show(argc, argv);
    next_cb(G_wiz, nullptr);
    return Fl::run();
}catch(std::exception& error){
    showError(error.what());
}
