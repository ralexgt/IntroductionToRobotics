// shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int startingButtonPin = 2;
const int lapButtonPin = 3;
const int resetButtonPin = 9;

int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; 

const int stopWatchButtonsCount = 3;
const int stopWatchButtons[stopWatchButtonsCount] = {startingButtonPin, lapButtonPin, resetButtonPin};

const int encodingsNumber = 10;

byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP
B11111100, // 0
B01100000, // 1
B11011010, // 2
B11110010, // 3
B01100110, // 4
B10110110, // 5
B10111110, // 6
B11100000, // 7
B11111110, // 8
B11110110, // 9
};

unsigned long lastIncrement = 0;
unsigned long delayCount = 100;
unsigned long numberDisplayed = 0;
unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 500;

byte paused = 1;
byte lapMode = 0;
byte reseted = 1;

const int numberOfLaps = 4;
int timeLaps[4] = {0, 0, 0, 0};
int savedLaps = 0;
int currentLap = -1;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  for (int i = 0; i < stopWatchButtonsCount; i++){
    pinMode(stopWatchButtons[i], INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(stopWatchButtons[0]), handlePause, FALLING);
  attachInterrupt(digitalPinToInterrupt(stopWatchButtons[1]), handleLap, FALLING);
  Serial.begin(9600);
}
void loop() {
  if(!paused){
    if (millis() - lastIncrement > delayCount) {
      numberDisplayed++;
      numberDisplayed %= 10000; // can not exceed 9999
      lastIncrement = millis();
    }
    writeNumber(numberDisplayed);
  }
  if(paused){
    resetStopWatch();
    if(!lapMode){
      writeNumber(numberDisplayed);
    }
    if(lapMode){
      writeNumber(timeLaps[currentLap]);
    }
  }
  startingScreen();

}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  // Start with the least significant digit
  int displayDigit = 3; 
  int lastDigit = 0;
  while (currentNumber != 0 || displayDigit >= 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigit);
    if(displayDigit == 2){
      writeReg(byteEncodings[lastDigit] + 1);
    } else {
      writeReg(byteEncodings[lastDigit]);
    }
    displayDigit--;
    currentNumber /= 10;
    // clear the register to avoid ghosting
    writeReg(B00000000); 
  }
}

void handlePause(){
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  if (interruptTime - lastInterruptTime > debounceDelay * 1000) {
    reseted = 0;
    paused = !paused;
    lapMode = 0;
    lastInterruptTime = interruptTime;
  }
}

void handleLap(){
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  if (interruptTime - lastInterruptTime > debounceDelay * 1000) {
    if(!paused){
      if(savedLaps == 4){
        savedLaps = 0;
      }
      timeLaps[savedLaps] = numberDisplayed;
      savedLaps++;
    }
    if(reseted){
      lapMode = 1;
      // start current lap at -1 so after incrementation we will get the first element from savedLaps
      if(currentLap == savedLaps - 1){
        currentLap = -1;
      }
      currentLap++;
    }
    lastInterruptTime = interruptTime;
  }
}

void resetStopWatch(){
  if(!digitalRead(resetButtonPin)){
    reseted = 1;
    if(!lapMode){
      for(int i = 0; i < displayCount; i++){
        numberDisplayed = 0;
      }
    }
    if(lapMode){
      savedLaps = 0;
     for(int i = 0; i < 4; i++){
        timeLaps[i] = 0;
      }
      lapMode = 0;
    }
  }
}

void startingScreen(){
  if(paused && reseted && !lapMode){
    writeNumber(0);
  }
}



