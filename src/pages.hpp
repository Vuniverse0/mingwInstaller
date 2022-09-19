#ifndef MINGWINSTALLER_PAGES_HPP
#define MINGWINSTALLER_PAGES_HPP

#include <vector>
#include <string>
#include <stdexcept>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>

constexpr size_t width = 650;
constexpr size_t height = 300;
constexpr size_t button_width = 150;
constexpr size_t button_height = 35;

void next_cb(Fl_Widget*, void*);
void back_cb(Fl_Widget*, void*);
void done_cb(Fl_Widget*, void*);
void downloadDone();

enum class ExcRs{ sjlj, dwarf, seh, error};
enum class MgRs{ win32, posix, error};
enum class Arcs{ i686, x86_64, error};

#endif //MINGWINSTALLER_PAGES_HPP
