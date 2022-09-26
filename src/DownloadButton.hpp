#ifndef MINGWINSTALLER_DOWNLOADBUTTON_HPP
#define MINGWINSTALLER_DOWNLOADBUTTON_HPP

#include <Fl/Fl_Button.H>


struct DownloadButton : Fl_Button {
    DownloadButton(int x, int y, int w, int h, const char *label = nullptr);
    void extracting();
    void downloading();
    void download();
};


#endif //MINGWINSTALLER_DOWNLOADBUTTON_HPP
