/*
   crc8.h

  (based on public elrs project code.)
*/

#ifndef _CRC8_H_
#define _CRC8_H_

#include <stdint.h>

class Crc8
{
  public:
    Crc8(uint8_t poly);
    uint8_t calc(uint8_t *data, uint8_t len);

  protected:
    uint8_t _lut[256];
    void init(uint8_t poly);
};

#endif // _CRC8_H_

// end crc8.h
