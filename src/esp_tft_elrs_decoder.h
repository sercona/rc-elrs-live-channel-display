/*
   esp_tft_elrs_decoder.h

   (c) 2023-2025 linux-works labs
*/


#ifndef _ESP_ELRS_DECODER_H_
#define _ESP_ELRS_DECODER_H_


// this is our main .ino filename (saved for showing build info via debug calls)
extern const char *MY_INO_FILE;

#define VERBOSE_SERIAL_TTY   // turn this OFF for production

#define PIN_RXD2          27 // 3 //9  // esp32-c3 supermini (connect this to elrs-tx out)
#define PIN_TXD2          1 //3  // leave this unconnected


#include <Arduino.h>
#include <EEPROM.h>
#include <SPI.h>

#include <TFT_eSPI.h>
extern TFT_eSPI tft;

#define LBUF_SIZE 256


#include "CrsfSerial.h"

extern HardwareSerial hwUart1;

// ignore 0, use 1..16
extern int chan_values[CRSF_NUM_CHANNELS+1];
extern int last_chan_values[CRSF_NUM_CHANNELS+1];


//
// tft color display
//

#include "ui_tft.h"

// constants for screen size and strip graph size
#ifdef SMALL_TFT
#define TFT_SIZE_H               (160)
#define TFT_SIZE_W               (128)
#endif

#define TFT_SIZE_H               (320)
#define TFT_SIZE_W               (240)



#endif // _ESP_ELRS_DECODER_H_


// end esp_tft_elrs_decoder.h
