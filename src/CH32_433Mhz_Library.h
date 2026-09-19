#ifndef CH32_433Mhz_Library_h
#define CH32_433Mhz_Library_h

#include <Arduino.h>

class CH32_433Mhz_Library {
  public:
    CH32_433Mhz_Library(int rxPin, int txPin);
    void begin();
    void sendByte(byte data);
    bool receiveByte(byte &outputByte);

  private:
    int _rxPin;
    int _txPin;
    void sendBurstToken();
    const byte _preambleKey = 0x12;
};

#endif
