/*
   ui_tft.h

   (c) 2023-2024 linux-works labs
*/


#ifndef _ui_tft_
#define _ui_tft_

#include <Arduino.h>

#include "esp_tft_elrs_decoder.h"             // our main include



class UI_TFT {

  public:

    // public methods
    void graphic_splash_screen_and_init(void);
    void draw_sercona_logo(void);
    void refresh_screen(void);
    void draw_labels(void);
    void update_bargraphs(void);


  private:
    // private methods
};

extern UI_TFT gui;



#endif // _ui_tft_ 
