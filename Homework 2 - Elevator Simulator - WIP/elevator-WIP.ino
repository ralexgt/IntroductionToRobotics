// cant upload

/* buzzer pin - 0 -- not implemented */

/* levelButtons[i] represents the 'i' level of the elevator */
const int levelButtonPins[3] = {2, 3, 11};
/* ledElevatorLevelPins[i] represents the pin for the 'i' level led */
const int ledElevatorLevelPins[3] = {8, 9, 10};
/* elevatorStateLed - always on, if the elevator starts moving - blinks */
const int elevatorStateLed = 12;

/* Flag to indicate button press event */
volatile bool buttonPressed = false;
/* Last time the ISR was triggered */
volatile unsigned long lastInterruptTime = 0;
/* Debounce time in milliseconds */
const unsigned long debounceDelay = 200;
/* duration for intermediar elevator level leds to blink */
const int blinkingTime = 50;

int currentLevel = 0;
int requestedLevel = 2;

/* track elevator state - HIGH or blinking */
byte ledElevatorState = HIGH;
/* states for led pin level 'i' */
int levelsLedState[3] = {0, 0, 0}; 

void setup() {
  for(currentLevel; currentLevel <= requestedLevel; currentLevel++){
    pinMode(levelButtonPins[currentLevel], INPUT_PULLUP);
  }
  currentLevel = 0;
  for(currentLevel; currentLevel <= requestedLevel; currentLevel++){
    pinMode(ledElevatorLevelPins[currentLevel], OUTPUT);
  }
  currentLevel = 0;
  pinMode(elevatorStateLed, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(levelButtonPins[0]), handleInterruptLevelZero, FALLING);
  attachInterrupt(digitalPinToInterrupt(levelButtonPins[1]), handleInterruptLevelOne, FALLING);
  attachInterrupt(digitalPinToInterrupt(levelButtonPins[2]), handleInterruptLevelTwo, FALLING);
  /* initialize elevator state led as HIGH */
  digitalWrite(elevatorStateLed, ledElevatorState); 
}

void loop() {
  if (buttonPressed) {
    digitalWrite(ledElevatorLevelPins[currentLevel], levelsLedState[currentLevel]);
    if(currentLevel < requestedLevel){
      for(currentLevel; currentLevel < requestedLevel; currentLevel++){
        ledBlink(ledElevatorLevelPins[currentLevel], levelsLedState[currentLevel]);
      }
      currentLevel = requestedLevel;
      digitalWrite(ledElevatorLevelPins[currentLevel], HIGH);
    }
      else if(currentLevel > requestedLevel){
        for(currentLevel; currentLevel > requestedLevel; currentLevel--){
        ledBlink(ledElevatorLevelPins[currentLevel], levelsLedState[currentLevel]);
      }
      currentLevel = requestedLevel;
      digitalWrite(ledElevatorLevelPins[currentLevel], HIGH);
    }

    /* reflag button press */
    buttonPressed = false;
  }
}

void ledBlink(int ledPin, byte ledState){
  for(int i = 0; i < blinkingTime; i++){
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}

void handleInterruptLevelZero() {
  /* handle interruptTime and update lastInterruptTime if the debounce delay is met */
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  /* set requested level that the elevator should stop at and flag button press as TRUE only if the elevator isn't already moving*/
  if(buttonPressed = false){
    if (interruptTime - lastInterruptTime > debounceDelay * 1000) {
      buttonPressed = true;
      requestedLevel = 0;
    }
    lastInterruptTime = interruptTime;
  }
}


void handleInterruptLevelOne() {
  /* handle interruptTime and update lastInterruptTime if the debounce delay is met */
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  /* set requested level that the elevator should stop at and flag button press as TRUE only if the elevator isn't already moving*/
  if(buttonPressed = false){
    if (interruptTime - lastInterruptTime > debounceDelay * 1000) {
      buttonPressed = true;
     requestedLevel = 1;
    }
    lastInterruptTime = interruptTime;
  }
}

void handleInterruptLevelTwo() {
  /* handle interruptTime and update lastInterruptTime if the debounce delay is met */
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  /* set requested level that the elevator should stop at and flag button press as TRUE only if the elevator isn't already moving*/
  if(buttonPressed = false){
    if (interruptTime - lastInterruptTime > debounceDelay * 1000) {
      buttonPressed = true;
     requestedLevel = 1;
    }
    lastInterruptTime = interruptTime;
  }
}













