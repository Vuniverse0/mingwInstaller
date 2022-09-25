#include "pages.hpp"

#include "Manager.hpp"

#include <Fl/Fl_Text_Display.H>
#include <Fl/Fl_Help_View.H>


///hello page

void page_()
{
    Fl_Group *g = new Fl_Group(0, 0, width, height);
    Fl_Help_View* text = new Fl_Help_View(50+png_size, 1, 600, 275);
    text->value(  "<h1>Welcome to Mingw install wizard</h1>"
"<p>This installer was developed for Windows users of the MinGW-W64 compiler. "
"<a href=\"https://www.mingw-w64.org/\">MinGW-W64</a> is a native Windows compiler based "
"on the <a href=\"https://gcc.gnu.org/\">GNU C Compiler</a>. </p> <p>"
"Unlike MSVC or Intel C++ or any other proprietary compilers, "
"MinGW-W64 is an open source compiler which "
"allows any user to be sure that the compiler generates only "
"the machine code that is the result of compiling the source code "
"written by the user, without any covertly generated machine code.</p><p> "
"This is especially important for applications that was designed "
"to ensure the safety of a user in the information space.</p>");
    text->textsize(15);

    Fl_Button *about = new Fl_Button(png_size+50, 280, button_width, button_height, "About");
    about->callback(back_cb);

    Fl_Button *next = new Fl_Button(button_x+png_size, button_y, button_width, button_height, "Next @->");
    next->callback(next_cb);


    Fl_Button *back = new Fl_Button(button_x-button_width-20+png_size, 280, button_width, button_height, "Exit");
    back->callback(done_cb);

    Manager::logo();

    g->end();
}