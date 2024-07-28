/*
   ui_tft.cpp

   (c) 2023-2024 linux-works labs
*/

#include "ui_tft.h"

#define x_off     11
#define max_width 86


// redraw the screen
void UI_TFT::refresh_screen (void)
{
  draw_labels();
  update_bargraphs();
}


// update dynamic data
void UI_TFT::update_bargraphs (void)
{
  char l_buf[LBUF_SIZE];
  int color;
  int mapped_chan_val;


  //
  // draw 8 channel bargraphs
  //


  // graphs
  for (int chan = 1; chan <= 8; chan++) {


    // did value change?

    if (chan_values[chan] != last_chan_values[chan]) {

      last_chan_values[chan] = chan_values[chan];    // save for compare next loop

      int y_pos = 15 + (chan - 1) * 17;

      // alternate colors
      if (chan == 5) {  // special for ARM channel
        color = TFT_RED;

      } else if (( chan % 2) == 0) {
        color = TFT_PINK; //GREEN; //YELLOW;

      } else {
        color = TFT_OLIVE;
      }

      tft.setTextColor(color, TFT_BLACK);


      // move the bottom 4 chans down a bit more
      if (chan >= 5) {
        y_pos += 7;
      }


      // map channel value
      mapped_chan_val = map(chan_values[chan], 1000, 2000, 0, max_width);

      // keep in range and make lower limit more visible
      if (mapped_chan_val < 0) {
        mapped_chan_val = 2;
      } else if (mapped_chan_val > max_width) {
        mapped_chan_val = max_width;
      }


      //
      // draw bargraphs
      //

      // draw outline
      tft.drawRoundRect(x_off, y_pos,  max_width, 10,
                        2, // radius
                        color);

      // draw value as filled-in area
      tft.fillRoundRect(x_off + 1, y_pos + 1,  mapped_chan_val, 8,
                        2, // radius
                        color);

      // draw rest of value as blacked-out area
      tft.fillRoundRect(x_off + mapped_chan_val + 1, y_pos + 1,  max_width - mapped_chan_val - 2, 8,
                        2, // radius
                        TFT_BLACK);


      // print actual value
      sprintf(l_buf, "%4d", chan_values[chan]);
      tft.setCursor(102, y_pos + 1);
      tft.print(l_buf);
    }
  }
}


// draw once (static labels)
void UI_TFT::draw_labels (void)
{
  char l_buf[LBUF_SIZE];
  int color;


  // legend text
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);

  // min, mid, max labels
  tft.setCursor(11, 0);
  tft.print("1000");

  tft.setCursor(43, 0);
  tft.print("1500");

  tft.setCursor(74, 0);
  tft.print("2000");


  // graph channel numbers
  for (int chan = 1; chan <= 8; chan++) {

    int y_pos = 15 + (chan - 1) * 17;

    // alternate colors
    if (chan == 5) {  // special for ARM channel
      color = TFT_RED;

    } else if (( chan % 2) == 0) {
      color = TFT_PINK; //GREEN; //YELLOW;

    } else {
      color = TFT_OLIVE;
    }

    tft.setTextColor(color, TFT_BLACK);

    // move the bottom 4 chans down a bit more
    if (chan >= 5) {
      y_pos += 7;
    }

    // chan num
    tft.setTextColor(color, TFT_BLACK);
    sprintf(l_buf, "%d", chan);
    tft.setCursor(0, y_pos + 1);
    tft.print(l_buf);
  }
}


#define LOGO_WIDTH      5
#define LOGO_HEIGHT     6
#define LOGO_LEFT_EDGE 36
#define LOGO_Y_OFFSET   4

void UI_TFT::draw_sercona_logo (void)
{
  uint8_t x, y;

  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(22, 46 + LOGO_Y_OFFSET); tft.print("Sercona");

  x = LOGO_LEFT_EDGE;
  tft.fillRoundRect(LOGO_LEFT_EDGE - 4, LOGO_Y_OFFSET,
                    64, 44,
                    3,  // radius
                    TFT_BLUE);


  // top chip-select trace
  x = LOGO_LEFT_EDGE; y = 6 + LOGO_Y_OFFSET;
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);  x += LOGO_WIDTH;
  tft.drawLine(x, y, x, y + LOGO_HEIGHT, TFT_CYAN);  y += LOGO_HEIGHT;

  tft.drawLine(x, y, x + 45, y, TFT_CYAN);  x += 45;

  tft.drawLine(x, y, x, y - LOGO_HEIGHT, TFT_CYAN);  y -= LOGO_HEIGHT;
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);  x += LOGO_WIDTH;


  // middle clock trace
  x = LOGO_LEFT_EDGE; y = 18 + LOGO_Y_OFFSET;
  for (int i = 1; i <= 5; i++) {
    tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);   x += LOGO_WIDTH;
    tft.drawLine(x, y, x, y + LOGO_HEIGHT, TFT_CYAN);  y += LOGO_HEIGHT;
    tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);   x += LOGO_WIDTH;
    tft.drawLine(x, y, x, y - LOGO_HEIGHT, TFT_CYAN);  y -= LOGO_HEIGHT;
  }
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);


  // bottom data trace
  x = LOGO_LEFT_EDGE; y = 30 + LOGO_Y_OFFSET;
  tft.drawLine(x, y, x + LOGO_WIDTH * 4, y, TFT_CYAN);  x += (LOGO_WIDTH * 4); // 1
  tft.drawLine(x, y, x, y + LOGO_HEIGHT, TFT_CYAN);  y += LOGO_HEIGHT;        // 2
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);  x += LOGO_WIDTH;          // 3
  tft.drawLine(x, y, x, y - LOGO_HEIGHT, TFT_CYAN);  y -= LOGO_HEIGHT;        // 4
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);  x += LOGO_WIDTH;          // 5
  tft.drawLine(x, y, x, y + LOGO_HEIGHT, TFT_CYAN);  y += LOGO_HEIGHT;        // 6
  tft.drawLine(x, y, x + LOGO_WIDTH * 4, y, TFT_CYAN);  x += (LOGO_WIDTH * 4); // 7
  tft.drawLine(x, y, x, y - LOGO_HEIGHT, TFT_CYAN);  y -= LOGO_HEIGHT;        // 8
  tft.drawLine(x, y, x + LOGO_WIDTH, y, TFT_CYAN);  x += LOGO_WIDTH;          // 9

  tft.setTextSize(1);   // leave things back in normal text size
}


void UI_TFT::graphic_splash_screen_and_init (void)
{
  //
  // splash screen
  //

  // proper way to clear the screen, it seems
  tft.fillRect(0, 0, TFT_SIZE_W, TFT_SIZE_H, TFT_BLACK);

  draw_sercona_logo();

  tft.setTextSize(1);
  tft.setCursor(5, 82);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print("ELRS Channel Monitor");

  tft.setCursor(2, 98);
  tft.print("(c) 2024 linux-works");

  delay(1000);
}




// end ui_tft.cpp
