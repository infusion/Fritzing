#include <TimerOne.h>

uint8_t Throttle = 0; // 0-127
uint8_t Yaw = 63; // 0-127, center=63
uint8_t Pitch = 63; // 0-127, center=63
uint8_t Trim = 63; // 0-127, center=63

#define SET_HIGH(t)    TCCR2A |= _BV(COM2B1); delayMicroseconds(t) // set pwm active on pin 3
#define SET_LOW(t)     TCCR2A &= ~_BV(COM2B1); delayMicroseconds(t) // set pwm inactive on pin3
#define SET_LOW_FINAL()   TCCR2A &= ~_BV(COM2B1)

void sendCommand(uint8_t yaw, uint8_t pitch, uint8_t throttle, uint8_t trim, uint8_t channel) {
    uint8_t data[4];

    data[3] = yaw;
    data[2] = pitch;
    data[1] = throttle | (channel << 7);
    data[0] = trim;

    // SEND HEADER

    SET_HIGH(2000);

    SET_LOW(2000);

    // SEND DATA

    for (int8_t i = 31; i >= 0; i--) {

        SET_HIGH(300); // 312?

        if ((data[i / 8] >> (i % 8)) % 2) {
            SET_LOW(700); // 712?
        } else {
            SET_LOW(300); // 312?
        }
    }

    // SEND FOOTER

    // 0.3ms HIGH
    SET_HIGH(300); // 312?

    // LOW till the next interrupt kicks in
    SET_LOW_FINAL();
}


void timerISR() {

    sendCommand(Yaw, Pitch, Throttle, Trim, 0);
}

void setup() {

    Serial.begin(9600);

    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);

    //setup interrupt interval: 180ms
    Timer1.initialize(180000);
    Timer1.attachInterrupt(timerISR);

    //setup PWM: f=38Khz PWM=50%
    // COM2A = 00: disconnect OC2A
    // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
    // WGM2 = 101: phase-correct PWM with OCRA as top
    // CS2 = 000: no prescaling
    TCCR2A = _BV(WGM20);
    TCCR2B = _BV(WGM22) | _BV(CS20);

    // Timer value
    OCR2A = 8000 / 38;
    OCR2B = OCR2A / 2; // 50% duty cycle
}

uint8_t state = 0;
void loop() {

  if (Serial.available() > 0) {

    uint8_t r = (uint8_t) Serial.read();

    switch (state) {
      case 0: if (r == 0xFF) state++; break;
      case 1: if (r == 0xAA) state++; break;
      case 2: Throttle = r; state++; break;
      case 3: Yaw = r; state++; break;
      case 4: Pitch = r; state++; break;
      case 5: Trim = r; state = 0; break;
    }
  }
}
