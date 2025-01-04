/*
   esp_tft_elrs_decoder.h

   (c) 2023-2024 linux-works labs
*/


#ifndef _ESP_ELRS_DECODER_H_
#define _ESP_ELRS_DECODER_H_

// this is our main .ino filename (saved for showing build info via debug calls)
extern const char *MY_INO_FILE;


//#define VERBOSE_SERIAL_TTY   // turn this OFF for production

// ----
// NOTE: define this FIRST before you include the TFT_eSPI.h file
// ----
// #define ST7735_INITB       // not working
#define ST7735_GREENTAB       // mostly works (noise on edge)
// #define ST7735_GREENTAB2   // mostly works (noise on edge)
// #define ST7735_GREENTAB3   // mostly works (noise on edge)
// #define ST7735_REDTAB      // mostly works (noise on edge)
// #define ST7735_BLACKTAB




#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>


#include <TFT_eSPI.h>
extern TFT_eSPI      tft;

#define LBUF_SIZE 1024

#include "ui_tft.h"

#include "CrsfSerial.h"


extern int chan_values[9];
extern int last_chan_values[9];


//
// tft color display
//

// constants for screen size and strip graph size
#define TFT_SIZE_H               (160)
#define TFT_SIZE_W               (128)





//
// FOR DOCS PURPOSES: physical and logical pin mapping for this application
//

// +++ LEAVE THESE UNCHANGED, PLEASE
//      name           GPIO    Arduino     Descr
//                     ADC0    // A0       // analog-in (UNUSED)
//                       1     // TX       // (UNUSED)
//                       3     // RX       // elrs rx input (from elrs module to esp8266)
//                      16     // D0       // (UNUSED)
//                       5     // D1       // i2c_clk (UNUSED)
//                       4     // D2       // i2c_data (UNUSED)
//                       0     // D3       // D/C or R/S or A0 on TFT
//                       2     // D4       // SPI_CS for sd-card (alternate SPI CS)
//                      14     // D5       // SPI_CLK for (TFT, SD)
//                      12     // D6       // SPI_MISO for (TFT, SD)
//                      13     // D7       // SPI_MOSI for (TFT, SD)
//                      15     // D8       // SPI_CS for TFT display
// --- LEAVE THESE UNCHANGED, PLEASE



#endif // _ESP_ELRS_DECODER_H_


// end esp_tft_elrs_decoder.h
