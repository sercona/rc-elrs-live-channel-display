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

UI_TFT      gui;               // GUI class for this app


// controller rx values (1-8, ignore 0)
int chan_values[9];
int last_chan_values[9];


#define PIN_LED    2   // D4
#define LED_OFF    1
#define LED_ON     0


// remove any previous def
#ifdef CRSF_BAUDRATE
#undef CRSF_BAUDRATE
#endif

#define CRSF_BAUDRATE 420000  // this is ok to use on both esp8266 and esp32

CrsfSerial crsf(Serial, CRSF_BAUDRATE);   // using the ONLY hardware serial on esp8266 (so remove your elrs rx if you have to re-flash the esp)



void packetChannels (void)
{
  for (int chan = 1; chan <= 8; chan++) {
    chan_values[chan] = crsf.getChannel(chan);
  }
}


void crsfLinkUp (void)
{
  digitalWrite(PIN_LED, 0);

#if 0
  // make link-up sound
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_BUZZER, 0);  // ground the pin, to turn the buzzer on
    delay(50);
    digitalWrite(PIN_BUZZER, 1);  // raise the pin, to turn the buzzer off
    delay(50);
  }
#endif
}


void crsfLinkDown (void)
{
  digitalWrite(PIN_LED, 1);

  // failsafe middle values
  for (int i = 0; i < 9; i++) {
    chan_values[i] = 1500;
    last_chan_values[i] = -1;  // force a change-detect on first loop
  }

#if 0
  // make link-down sound
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_BUZZER, 0);  // ground the pin, to turn the buzzer on
    delay(50);
    digitalWrite(PIN_BUZZER, 1);  // raise the pin, to turn the buzzer off
    delay(50);
  }
#endif
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
  for (int i = 0; i < 9; i++) {
    chan_values[i] = 1500;
    last_chan_values[i] = -1;  // force a change-detect on first loop
  }

  // init color display
  tft.init();
  tft.setRotation(0);
  tft.setTextSize(1);

  gui.graphic_splash_screen_and_init();

  // proper way to clear the screen, it seems
  tft.fillRect(0, 0, TFT_SIZE_W, TFT_SIZE_H, TFT_BLACK);

  // initial tft display field headings
  gui.refresh_screen();


  // start the crossfire class
  crsf.begin();

  // attach the channels callback
  crsf.onPacketChannels = &packetChannels;
  crsf.onLinkUp = &crsfLinkUp;
  crsf.onLinkDown = &crsfLinkDown;
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
    gui.update_bargraphs();
    yield();

    last_millis = millis();   // record when we finished, for more even spacing between graph displays
  }

} // loop




// end esp_tft_elrs_decoder.ino
