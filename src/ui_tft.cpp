/*
   ui_tft.cpp

   (c) 2023-2024 linux-works labs
*/

#include "ui_tft.h"

#define x_off     16
#define max_width 64   // larger bargraphs

int top_offset = 42;
int bargraph_spacing = 17;
int bargraph_height = 17;



// redraw the screen
void UI_TFT::refresh_screen (void)
{
  update_sticks(false);  // dont erase stick area automatically
}



// update dynamic data
void UI_TFT::update_sticks (bool clear_stick_display)
{
  char l_buf[LBUF_SIZE];
  int color;
  int mapped_chan_val;

  int circle_radius = 45;
  int center_y = 50;
  int center_x1 = 60 - 8;
  int center_x2 = 16 + 2 * circle_radius + center_x1;

  int y_pos;
  int more_x;

  // min,max x,y graph limits for each stick
  int stick1_min_x = center_x1 - circle_radius + 1;  // just inside the circle
  int stick1_max_x = center_x1 + circle_radius - 1;  // just inside the circle
  int stick1_min_y = center_y  - circle_radius + 1;  // just inside the circle
  int stick1_max_y = center_y  + circle_radius - 1;  // just inside the circle

  int stick2_min_x = center_x2 - circle_radius + 1;  // just inside the circle
  int stick2_max_x = center_x2 + circle_radius - 1;  // just inside the circle
  int stick2_min_y = center_y  - circle_radius + 1;  // just inside the circle
  int stick2_max_y = center_y  + circle_radius - 1;  // just inside the circle


  //
  // draw both sticks at the top of the screen
  //

  // check if this is our first time thru.  dont render '-1' stick values; wait until we get proper values
  if (last_chan_values[1] == (-1) ||
      last_chan_values[2] == (-1) ||
      last_chan_values[3] == (-1) ||
      last_chan_values[4] == (-1)) {

    // copy current to last-saved
    for (int j = 1; j <= 4; j++) {
      last_chan_values[j] = chan_values[j];
    }
  }


  // left circle
  //tft.drawCircle(center_x1, center_y, circle_radius, TFT_WHITE);

  if (clear_stick_display) {
    tft.fillRoundRect(stick1_min_x, stick1_min_y,
                      (stick1_max_x - stick1_min_x),
                      (stick1_max_y - stick1_min_y),
                      5, TFT_BLACK);
  }
  tft.drawRoundRect(stick1_min_x, stick1_min_y,
                    (stick1_max_x - stick1_min_x),
                    (stick1_max_y - stick1_min_y),
                    5, TFT_WHITE);


  // right circle
  //tft.drawCircle(center_x2, center_y, circle_radius, TFT_WHITE);

  if (clear_stick_display) {
    tft.fillRoundRect(stick2_min_x, stick2_min_y,
                      (stick2_max_x - stick2_min_x),
                      (stick2_max_y - stick2_min_y),
                      5, TFT_BLACK);
  }

  tft.drawRoundRect(stick2_min_x, stick2_min_y,
                    (stick2_max_x - stick2_min_x),
                    (stick2_max_y - stick2_min_y),
                    5, TFT_WHITE);

  // tick marks at 12, 3, 6, 9 oclock
  //tft.drawLine()




  // erase old stick dots (if not the first time thru)

  int old_stick1_x_mapped = map(last_chan_values[4], 1000, 2000, stick1_min_x + 2, stick1_max_x - 4);
  int old_stick1_y_mapped = map(last_chan_values[3], 2000, 1000, stick1_min_y + 2, stick1_max_y - 4); // invert direction

  int old_stick2_x_mapped = map(last_chan_values[1], 1000, 2000, stick2_min_x + 2, stick2_max_x - 4);
  int old_stick2_y_mapped = map(last_chan_values[2], 2000, 1000, stick2_min_y + 2, stick2_max_y - 4); // invert direction

  if (!clear_stick_display) {
    // render/erase old stick lines (if not the first time thru)
    tft.drawLine(center_x1 - 1, center_y + 2, old_stick1_x_mapped, old_stick1_y_mapped, TFT_BLACK);
    tft.drawLine(center_x2 - 1, center_y + 2, old_stick2_x_mapped, old_stick2_y_mapped, TFT_BLACK);

    // render/erase the 2 sticks as black dots (erasing them)
    tft.fillCircle(old_stick1_x_mapped, old_stick1_y_mapped, 4, TFT_BLACK);
    tft.fillCircle(old_stick2_x_mapped, old_stick2_y_mapped, 4, TFT_BLACK);
  }


  // new sticks as dots

  int stick1_x_mapped = map(chan_values[4], 1000, 2000, stick1_min_x + 2, stick1_max_x - 4);
  int stick1_y_mapped = map(chan_values[3], 2000, 1000, stick1_min_y + 2, stick1_max_y - 4); // invert direction

  int stick2_x_mapped = map(chan_values[1], 1000, 2000, stick2_min_x + 2, stick2_max_x - 4);
  int stick2_y_mapped = map(chan_values[2], 2000, 1000, stick2_min_y + 2, stick2_max_y - 4); // invert direction

  int fill_color;
  if (!link_down_status) {
    fill_color = TFT_RED;

    // render new stick lines (if not the first time thru)
    tft.drawLine(center_x1 - 1, center_y + 2, stick1_x_mapped, stick1_y_mapped, fill_color);
    tft.drawLine(center_x2 - 1, center_y + 2, stick2_x_mapped, stick2_y_mapped, fill_color);
  } else {
    fill_color = TFT_WHITE;
  }


  // render new sticks as dots
  tft.fillCircle(stick1_x_mapped, stick1_y_mapped, 4, fill_color);
  tft.fillCircle(stick2_x_mapped, stick2_y_mapped, 4, fill_color);

  // copy current to last-saved
  for (int j = 1; j <= 4; j++) {
    last_chan_values[j] = chan_values[j];
  }


  //
  // controller bargraphs for remaining channels
  //

  for (int chan = 5; chan <= 16; chan++) {

    // did value change?

    if (chan_values[chan] != last_chan_values[chan]) {

      last_chan_values[chan] = chan_values[chan];    // save for compare next loop

      if (chan <= 10) {
        y_pos = top_offset + (chan - 1) * bargraph_spacing;
        more_x = 2;
      } else {
        y_pos = top_offset + (chan - 6 - 1) * bargraph_spacing;  // wrap around to top of 2nd column
        more_x = 112; //67;
      }

      // alternate colors
      if (chan == 5) {  // special for ARM channel
        color = TFT_RED;

      } else if (( chan % 2) == 0) {
        color = TFT_PINK; //GREEN; //YELLOW;

      } else {
        color = TFT_OLIVE;
      }

      tft.setTextColor(color, TFT_BLACK);



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
      tft.drawRoundRect(x_off + more_x,
                        y_pos,
                        max_width, 10 - 2,
                        2, // radius
                        color);

      // draw value as filled-in area
      tft.fillRoundRect(x_off + more_x + 1,
                        y_pos + 1,
                        mapped_chan_val, 8 - 2,
                        2, // radius
                        color);

      // draw rest of value as blacked-out area
      tft.fillRoundRect(x_off + more_x + mapped_chan_val + 1,
                        y_pos + 1,
                        max_width - mapped_chan_val - 2, 8 - 2,
                        2, // radius
                        TFT_BLACK);

#if 0
      // print actual value
      sprintf(l_buf, "%4d", chan_values[chan]);
      tft.setCursor(102, y_pos + 1);
      tft.print(l_buf);
#endif
    }
  }
}


// draw once (static labels)
void UI_TFT::draw_labels (void)
{
  char l_buf[LBUF_SIZE];
  int color;
  int y_offset = 80;  // where to put the labels (y-axis)



  // legend text
  tft.setTextSize(1);
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);

#ifdef NOT
  // min, mid, max labels
  tft.setCursor(11, y_offset);
  tft.print("1000");

  tft.setCursor(43, y_offset);
  tft.print("1500");

  tft.setCursor(74, y_offset);
  tft.print("2000");
#endif


  // graph channel numbers

  // start out with channel 5 (first 4 go to the sticks)

  for (int chan = 5; chan <= 10; chan++) {

    int y_pos = (top_offset - 7) + (chan - 1) * bargraph_spacing;

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

    tft.setTextColor(color, TFT_BLACK);

    // left column chan num
    sprintf(l_buf, "%2d", chan);
    tft.setCursor(1, y_pos + 1);
    tft.print(l_buf);

    // right column chan num

    // hack to disable red for this channel on the right (#11)
    if (chan == 5) {
      tft.setTextColor(TFT_OLIVE, TFT_BLACK);
    } else {
      tft.setTextColor(color, TFT_BLACK);
    }

    sprintf(l_buf, "%2d", chan + 6);
    tft.setCursor(/*66*/110, y_pos + 1);
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
