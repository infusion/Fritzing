
// Robert Eisele https://github.com/infusion

#define ECHOPIN 2
#define TRIGPIN 3

void setup() {

  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGPIN, OUTPUT);
}

void loop() {

  long duration, distance;

  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);

  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration - 29) / 58; // AVG 0,4cm off

  // use: distance

  delay(200);
}

