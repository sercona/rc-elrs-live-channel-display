/*
  esp_tft_elrs_decoder

  arduino esp8266 (also can work with esp32) that receives a 'crossfire' (elrs) serial stream
  from a common, standard elrs serial receiver module and decodes it into 8 channels
  that are displayed separately on a tft display.

  elrs code based on open code from the 'express LRS' project.

  (c) 2024 linux-works labs
*/

const char *MY_INO_FILE = __FILE__;  // capture the .ino filename and save it

#include "esp_tft_elrs_decoder.h"             // our main include


//
// library classes
//

TFT_eSPI tft = TFT_eSPI();                   // tft display (one instance)

//
// app-specific classes
//

UI_TFT gui;               // GUI class for this app


// controller rx values (1-16, ignore 0)
int chan_values[17];
int last_chan_values[17];

#define LED_BUILTIN 4  // CYD
#define PIN_LED    LED_BUILTIN
#define LED_OFF    1
#define LED_ON     0

// remove any previous def
#ifdef CRSF_BAUDRATE
#undef CRSF_BAUDRATE
#endif

#define CRSF_BAUDRATE 420000  // this is ok to use on both esp8266 and esp32

HardwareSerial hwUart1(1);

CrsfSerial crsf(hwUart1, CRSF_BAUDRATE);   // using the ONLY hardware serial on esp8266 (so remove your elrs rx if you have to re-flash the esp)

bool link_down_status;


void packetChannels (void)
{
  for (int chan = 1; chan <= 16; chan++) {
    chan_values[chan] = crsf.getChannel(chan);
  }
}


void crsfLinkUp (void)
{
  digitalWrite(PIN_LED, 0);
  link_down_status = false;
}


void crsfLinkDown (void)
{
  digitalWrite(PIN_LED, 1);

  link_down_status = true;

  // failsafe middle values
  for (int i = 0; i < 17; i++) {
    chan_values[i] = 1500;
    last_chan_values[i] = -1;  // force a change-detect on first loop
  }


  gui.update_sticks(true);   // clear out previous data for sticks
}



void setup (void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);  // turn off led


  //
  // setup serial tty out (or not)
  //

#ifdef VERBOSE_SERIAL_TTY
  Serial.setDebugOutput(true);
  Serial.begin(115200);
  while (!Serial) {
    delay(100);      // will pause Zero, Leonardo, etc until serial console opens
  }
#else
  Serial.setDebugOutput(false);
#endif


  // init channels
  for (int i = 0; i < 17; i++) {
    chan_values[i] = 1500;
    last_chan_values[i] = -1;  // force a change-detect on first loop
  }
  
  link_down_status = true;


  // init color display
  tft.init();
  tft.setRotation(2);
  tft.setTextSize(1);

  gui.graphic_splash_screen_and_init();

  // proper way to clear the screen, it seems
  tft.fillRect(0, 0, TFT_SIZE_W, TFT_SIZE_H, TFT_BLACK);

  // initial tft display field headings
  //gui.refresh_screen();
  gui.draw_labels();

  // use a 2nd hardware uart for elrs
  hwUart1.begin(CRSF_BAUDRATE, SERIAL_8N1, RXD2, TXD2);

  // start the crossfire class
  crsf.begin();

  // attach the channels callback
  crsf.onPacketChannels = &packetChannels;
  crsf.onLinkUp         = &crsfLinkUp;
  crsf.onLinkDown       = &crsfLinkDown;
}




//
// the main arduino loop
//

void loop (void)
{
  static int last_millis = 0;


  // get channel values from receiver
  crsf.loop();
  yield();


  // run the tft screen update on a timer
  if ((millis() - last_millis) > 20) {
    gui.refresh_screen();
    yield();

    last_millis = millis();   // record when we finished, for more even spacing between graph displays
  }

} // loop




// end esp_tft_elrs_decoder.ino
