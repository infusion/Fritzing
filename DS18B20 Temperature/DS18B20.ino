
// Robert Eisele https://github.com/infusion
// Based on http://www.elecrow.com/wiki/index.php?title=One_Wire_Waterproof_Temperature_Sensor

#include <OneWire.h>


#define DS18S20_PIN 2

OneWire ds(DS18S20_PIN);

float readTemperature() {

  byte data[12];
  byte addr[8];

  byte type_s;

  if (!ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return NAN;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    //CRC is not valid
    return NAN;
  }

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      // DS18S20 or old DS1820
      type_s = 1;
      break;
    case 0x28:
      // DS18B20
      type_s = 0;
      break;
    case 0x22:
      // DS1822
      type_s = 0;
      break;
    default:
      // Device is not a DS18x20 family device
      return NAN;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);

  delay(1000);

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  // convert the data to actual temperature

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3; // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  return (float) raw / 16.0;
}

void setup() {
  
}

void loop() {

  float temperature = readTemperature();

  // Do something with temperature

}
