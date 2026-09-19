#include "CH32_433Mhz_Library"

TimeSliceRF::TimeSliceRF(int rxPin, int txPin) {
  _rxPin = rxPin;
  _txPin = txPin;
}

void TimeSliceRF::begin() {
  pinMode(_rxPin, INPUT);
  pinMode(_txPin, OUTPUT);
}

void TimeSliceRF::sendBurstToken() {
  for (int i = 0; i < 20; i++) {
    digitalWrite(_txPin, HIGH); delay(1);
    digitalWrite(_txPin, LOW);  delay(1);
  }
}

void TimeSliceRF::sendByte(byte data) {
  // Transmit validation signature followed directly by the data byte
  byte package[2] = {_preambleKey, data};
  
  for (int b = 0; b < 2; b++) {
    for (int i = 7; i >= 0; i--) {
      sendBurstToken();
      if (bitRead(package[b], i) == 0) {
        delay(10);
      } else {
        delay(30);
      }
    }
  }
}

bool TimeSliceRF::receiveByte(byte &outputByte) {
  unsigned long duration = pulseIn(_rxPin, HIGH, 50000UL);

  if (duration == 0) {
    unsigned long flushStart = micros();
    while (micros() - flushStart < 2000UL) {
      digitalRead(_rxPin); 
    }
    return false; 
  }

  if (duration > 850 && duration < 1150) {
    bool timeline[100]; 
    for(int s = 0; s < 100; s++) timeline[s] = false;

    for (int slice = 0; slice < 100; slice++) {
      int highSamples = 0;
      int totalSamples = 0;
      unsigned long sliceStart = millis();

      while (millis() - sliceStart < 10) {
        if (digitalRead(_rxPin) == HIGH) highSamples++;
        totalSamples++;
      }

      if (totalSamples > 0 && ((highSamples * 100) / totalSamples) > 25) {
        timeline[slice] = true;
      }
    }

    byte decodedBytes[2] = {0, 0};
    int byteIdx = 0;
    int bitIndex = 7;
    int lastBurstSlice = 0;
    bool clusterFound = false;
    bool success = true;

    for (int s = 0; s < 100; s++) {
      if (timeline[s]) {
        lastBurstSlice = s;
        clusterFound = true;
        break;
      }
    }

    if (clusterFound) {
      for (int s = lastBurstSlice + 1; s < 100; s++) {
        if (timeline[s] && !timeline[s - 1]) {
          int sliceGap = s - lastBurstSlice;
          lastBurstSlice = s;

          if (sliceGap >= 2 && sliceGap <= 5) {
            bitWrite(decodedBytes[byteIdx], bitIndex, 0);
            bitIndex--;
          } else if (sliceGap >= 6 && sliceGap <= 9) {
            bitWrite(decodedBytes[byteIdx], bitIndex, 1);
            bitIndex--;
          } else {
            success = false; 
            break;
          }

          if (bitIndex < 0) {
            byteIdx++;
            bitIndex = 7;
            if (byteIdx > 1) break; 
          }
        }
      }
    }

    if (success && byteIdx >= 2 && decodedBytes[0] == _preambleKey) {
      outputByte = decodedBytes[1];
      return true;
    }
  }
  return false;
}
