#ifndef MINGWINSTALLER_PAGES_HPP
#define MINGWINSTALLER_PAGES_HPP

#include "BuildInfo.hpp"

constexpr std::size_t png_size = 300;
constexpr std::size_t width = 650 + 300;
constexpr std::size_t height = 310;
constexpr std::size_t button_width = 100;
constexpr std::size_t button_height = 25;
constexpr std::size_t button_x = 540;
constexpr std::size_t button_y = 280;

#include <FL/Fl.H>
#include <Fl/Fl_Widget.H>

void next_cb(Fl_Widget*, void*);
void back_cb(Fl_Widget*, void*);
void done_cb(Fl_Widget*, void*);

#endif //MINGWINSTALLER_PAGES_HPP
