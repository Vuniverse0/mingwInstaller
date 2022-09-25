#ifndef MINGWINSTALLER_PAGES_HPP
#define MINGWINSTALLER_PAGES_HPP

#include <vector>
#include <string>
#include <stdexcept>

constexpr size_t width = 650 + 300;
constexpr size_t height = 300 + 10;
constexpr size_t png_size = 300;
constexpr size_t button_width = 100;
constexpr size_t button_height = 25;
constexpr size_t button_x = 540;
constexpr size_t button_y = 280;

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Choice.H>

void next_cb(Fl_Widget*, void*);
void back_cb(Fl_Widget*, void*);
void done_cb(Fl_Widget*, void*);

#endif //MINGWINSTALLER_PAGES_HPP
