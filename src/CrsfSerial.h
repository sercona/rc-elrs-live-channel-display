/*
   CrsfSerial.h

   (based on public elrs project code.)
*/

#ifndef _CRSF_SERIAL_H_
#define _CRSF_SERIAL_H_

#include <Arduino.h>

#include "crc8.h"
#include "crsf_protocol.h"

enum eFailsafeAction { fsaNoPulses, fsaHold };

class CrsfSerial
{
  public:
    // Packet timeout where buffer is flushed if no data is received in this time
    static const unsigned int CRSF_PACKET_TIMEOUT_MS = 100;
    static const unsigned int CRSF_FAILSAFE_STAGE1_MS = 300;

    CrsfSerial(HardwareSerial &port, uint32_t baud = CRSF_BAUDRATE);

    void begin(uint32_t baud = 0);

    void loop();

    void write(uint8_t b);
    void write(const uint8_t *buf, size_t len);

    void queuePacket(uint8_t addr, uint8_t type, const void *payload, uint8_t len);

    uint32_t getBaud (void) const {
      return _baud;
    };

    // Return current channel value (1-based) in us
    int getChannel (unsigned int ch) const {
      return _channels[ch - 1];
    }

    const crsfLinkStatistics_t *getLinkStatistics (void) const {
      return &_linkStatistics;
    }

    const crsf_sensor_gps_t *getGpsSensor (void) const {
      return &_gpsSensor;
    }

    const crsf_sensor_battery_t *getBatterySensor (void) const {
      return &_batterySensor;
    }

    bool isLinkUp (void) const {
      return _linkIsUp;
    }

    bool getPassthroughMode (void) const {
      return _passthroughBaud != 0;
    }

    void setPassthroughMode(bool val, uint32_t passthroughBaud = 0);

    // Event Handlers
    void (*onLinkUp)(void);
    void (*onLinkDown)(void);

    // OobData is any byte which is not CRSF, including passthrough
    void (*onOobData)(uint8_t b);

    // CRSF Packet Callbacks
    void (*onPacketChannels)(void);
    void (*onPacketLinkStatistics)(crsfLinkStatistics_t *ls);
    void (*onPacketGps)(crsf_sensor_gps_t *gpsSensor);
    void (*onPacketBattery)(crsf_sensor_battery_t *batterySensor);

  private:
    HardwareSerial       &_port;
    
    uint8_t               _rxBuf[CRSF_MAX_PACKET_SIZE];
    uint8_t               _rxBufPos;
    Crc8                  _crc;
    
    crsfLinkStatistics_t  _linkStatistics;
    crsf_sensor_gps_t     _gpsSensor;
    crsf_sensor_battery_t _batterySensor;

    uint32_t              _baud;
    uint32_t              _lastReceive;
    uint32_t              _lastChannelsPacket;
    bool                  _linkIsUp;
    uint32_t              _passthroughBaud;
    int                   _channels[CRSF_NUM_CHANNELS];

    void handleSerialIn(void);
    void handleByteReceived(void);
    void shiftRxBuffer(uint8_t cnt);
    void processPacketIn(uint8_t len);
    void checkPacketTimeout(void);
    void checkLinkDown(void);

    // Packet Handlers
    void packetChannelsPacked(const crsf_header_t *p);
    void packetLinkStatistics(const crsf_header_t *p);
    void packetGps(const crsf_header_t *p);
    void packetBattery(const crsf_header_t *p);
};

extern CrsfSerial crsf; //(hwUart1, CRSF_BAUDRATE);

#endif // _CRSF_SERIAL_H_

// end CrsfSerial.h
