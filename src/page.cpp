#include "pages.hpp"
#include <Fl/Fl_Text_Display.H>
#include <Fl/Fl_Help_View.H>


///hello page

void page_()
{
    Fl_Group *g = new Fl_Group(0, 0, width, height);

    Fl_Button *next = new Fl_Button(290, 265, 100, 25, "Next @->");
    next->callback(next_cb);

    Fl_Button *back = new Fl_Button(180, 265, 100, 25, "Exit");
    back->callback(done_cb);

    auto* logo = new Fl_PNG_Image("", logo_png, static_cast<int>(logo_png_len));
    auto* box = new Fl_Box( 0, 0, 50, 50);
    box->image(logo);

    Fl_Help_View* text = new Fl_Help_View(50, 5, 600, 230);
    text->value(  "<h1>Welcome to Mingw install wizard</h1>"
"<p>This installer was developed for Windows users of the MinGW-W64 compiler. "
"<a href=\"https://www.mingw-w64.org/\">MinGW-W64</a> is a native Windows compiler based "
"on the <a href=\"https://gcc.gnu.org/\">GNU C Compiler</a>. "
"Unlike MSVC or Intel C++ or any other proprietary compilers,"
"MinGW-W64 is an open source compiler which"
"allows any user to be sure that the compiler generates only"
"the machine code that is the result of compiling the source code "
"written by the user, without any covertly generated machine code. "
"This is especially important for applications that was designed"
"to ensure the safety of a user in the information space.</p>");
    text->textsize(15);

    g->end();
}
