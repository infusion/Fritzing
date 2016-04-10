
// Robert Eisele https://github.com/infusion

#define INTERVAL 1000

// 3144 is a binary switch
// We could either messure on a pin and read the sensor or better:
// Count asynchronously with an interrupt.

volatile unsigned int seen = 0;

void magnetSeen() {
  // Increment the seen counter
  seen++;
}

void setup() {

  // Attach callback
  attachInterrupt(0, magnetSeen, RISING);

}

void loop() {

  // Calculate revolutions per minute:
  unsigned int rpm = seen * 30 * 1000 / INTERVAL;

  // Reset counter
  seen = 0;

  // Do something with rpm

  // Sleep for a while
  delay(INTERVAL);
}

