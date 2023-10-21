const int inputRedPin = A3;
const int inputGreenPin = A4;
const int inputBluePin = A5;
const int redLedPin = 3;
const int greenLedPin = 5;
const int blueLedPin = 6;

const int minInputValue = 0;
const int maxInputValue = 1023;
const int minMappedLedValue = 0;
const int maxMappedLedValue = 255;

void setup() {
  pinMode(inputRedPin, INPUT);
  pinMode(inputGreenPin, INPUT);
  pinMode(inputBluePin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}

void loop() {
  int redLedValue = analogRead(inputRedPin);
  redLedValue = map(redLedValue, minInputValue, maxInputValue, minMappedLedValue, maxMappedLedValue);
  int greenLedValue = analogRead(inputGreenPin);
  greenLedValue = map(greenLedValue, minInputValue, maxInputValue, minMappedLedValue, maxMappedLedValue);
  int blueLedValue = analogRead(inputBluePin);
  blueLedValue = map(blueLedValue, minInputValue, maxInputValue, minMappedLedValue, maxMappedLedValue);

  analogWrite(redLedPin, redLedValue);
  analogWrite(greenLedPin, greenLedValue);
  analogWrite(blueLedPin, blueLedValue);
}
