# Arduino Display of ELRS Channel Values

![display](tft_channel_display.jpg)

* Purpose:
  - For remote control (RC) pilots who want to see what the values being sent by their transmitter (handset) are.
  - ExpressLRS (ELRS) is a new RC protocol that is becoming the dominant control protocol in the hobby.
  - To bring-up (build, fix, modify) flight controllers and other related hardware, its useful to have something that can see actual values in realtime.
  - ELRS uses the concept of 'binding' (roughly similar to bluetooth) to link stations; to view an ELRS transmitter stream, you must be using the same 'binding phrase' as the transmitter.


* Demo video: (https://github.com/sercona/rc-elrs-live-channel-display/blob/main/elrs-controller-demo1.mp4)

* Based on some demo code from the expresslrs project

* Hardware needed:
  - ESP8266 (eg, wemos d1 mini)
  - elrs serial receiver (as normally used in a quadcopter, etc)
  - SPI TFT display ST7735 (https://www.amazon.com/gp/product/B0C2HNVSWV/)
  - 3d printed case
  
* Software:
  - Import the code as you normally would with the arduino IDE
  - Flash your esp8266 over its usb serial port.  IMPORTANT: remove the elrs receiver when you serial-flash your ESP chip.
  - Once flash is done, the elrs receiver module can now be plugged in; with its 'tx' pin (this is the data OUT from the elrs module) connected to the 'rx' pin on the only serial uart of the ESP8266. 

* Connections:
  - Only 3 wires are needed for elrs listening; the data wire (to the RX gpio of the ESP), and 5v power and ground.
  - Power the ESP chip, tft display and elrs module from an external 5v supply.
  - If you need to reflash the ESP, remember to remove the elrs (or put a switch on the single data line) before you flash and reconnect after flash is done.
  - Wiring gpio pins can be found in src/esp_tft_elrs_decoder.h
    - TFT pins on wemos d1 mini: (D4=SPI_CS, D5=SPI_CLK, D6=SPI_MISO, D7=DPI_MOSI, D8=SPI_CS)
    - TFT will need backlight; wire that to 5v.  RESET wire on tft goes to RESET on ESP chip.  Note, TFT vcc is 3.3v not 5v!
  
  ![wiring](diy-build-wiring.png)

* 3D print STL files in the stl folder
  ![3dprints](3d-printables.png)

