#include "version.hpp"

#include "pages.hpp"

#include "Manager.hpp"

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <Fl/Fl_Help_View.H>


///page_about
void page_about()
{
    auto *g = new Fl_Group(0, 0, width, height);
    auto *text = new Fl_Help_View(50 + png_size, 1, 600, 275);
    text->value(("<h1>About</h1> <p>Version: "
                 MINGW_W64_INSTALLER_VERSION
                "</p><p>This installer was developed by <a href=\"https://github.com/Vuniverse0/\">Vuniverse</a>."
                "</p> <p> It is open <a href=\"https://github.com/Vuniverse0/mingwInstaller/\">source</a>."
                "<p><a href=\"https://github.com/Vuniverse0/mingwInstaller/issues\">Report a bug or an error.</a></p>"
                ).c_str());
    text->textsize(20);

    auto *next = new Fl_Button(button_x + png_size, button_y, button_width, button_height, "Ok");
    next->callback(next_cb);

    Manager::logo();

    g->end();
}