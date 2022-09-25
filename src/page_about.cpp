#include "pages.hpp"

#include "Manager.hpp"

#include <Fl/Fl_Text_Display.H>
#include <Fl/Fl_Help_View.H>
#include <Fl/Fl_Button.H>


///page_about
void page_about()
{
    auto *g = new Fl_Group(0, 0, width, height);

    auto* text = new Fl_Help_View(50+png_size, 1, 600, 275);

    text->value(  "<h1>About</h1>"
                  "<p>This installer was developed by <a href=\"https://github.com/Vuniverse0/\">Vuniverse</a>."
                  "</p> <p> It is open <a href=\"https://github.com/Vuniverse0/mingwInstaller/\">source</a>.");
    text->textsize(20);

    auto *next = new Fl_Button(button_x + png_size, button_y, button_width, button_height, "Ok");
    next->callback(next_cb);

    Manager::logo();

    g->end();
}
//TODO add my contacts like email
