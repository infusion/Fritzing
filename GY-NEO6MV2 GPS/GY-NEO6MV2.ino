
// Robert Eisele https://github.com/infusion

#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(3, 4); // RX, TX
TinyGPS gps;

void setup()  {
  mySerial.begin(9600);
}

void loop() {
  bool ready = false;
  if (mySerial.available()) {
    char c = mySerial.read();
    if (gps.encode(c)) {
      ready = true;
    }
  }

  // Use actual data
  if (ready) {
    // Use `gps` object
  }
}
