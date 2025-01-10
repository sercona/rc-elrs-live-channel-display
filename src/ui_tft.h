/*
   ui_tft.h

   (c) 2023-2025 linux-works labs
*/

#ifndef _UI_TFT_H_
#define _UI_TFT_H_

#include <Arduino.h>

#include "esp_tft_elrs_decoder.h"             // our main include

class UI_TFT {

  public:

    // public methods
    void graphic_splash_screen_and_init(void);
    void draw_sercona_logo(void);
    void refresh_screen(void);
    void draw_labels(void);
    void update_sticks(bool clear_stick_display);

  private:
    // private methods
};

extern UI_TFT gui;


#endif // _UI_TFT_H_ 
