// Declare all the joystick pins
const int pinSW = 2; // 2 - digital pin connected to switch output (pushbutton)
const int pinX = A0; // A0 - analog pin connected to X axis output
const int pinY = A1; // A1 - analog pin connected to Y axis output

// declare all the segments' pins
const int pinA = 11;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
// declare each segment's state
int statePinA = 0;
int statePinB = 0;
int statePinC = 0;
int statePinD = 0;
int statePinE = 0;
int statePinF = 0;
int statePinG = 0;
int statePinDP = 0;

// current and last blink times for led blink function
unsigned long currentBlinkMillis = 0;
unsigned long lastBlinkMillis = 0;
// each "blinkingTIme" miliseconds the current segment should switch states
const int blinkingTime = 300;
// debounce time for joystick input
const int joystickDebounce = 300;
unsigned long lastJoyMove = 0;
unsigned long currentTime = 0;

// time variables for the interrupt function
unsigned long lastInterruptTime = 0;
unsigned long switchDebounce = 100;
// if the switch is pressed more than "swResetTime" miliseconds the display should reset
const int swResetTime = 1000;
unsigned long pressTime = 0;
unsigned long releaseTime = 0;

// states for 7-segments LEDs and joystick's switch
byte state = LOW;
byte swState = LOW;
byte lastSwState = LOW;
// variables for x and y axis on joystick
int xValue = 0;
int yValue = 0;

// toggle when the joystick was moved
bool joyMoved = false;
// min and max thresholds for joystick's output values
const int minThreshold = 100;
/* maximum threshold is set high because when the switch is pressed, the x axis value goes gradually from ~500 to ~1020,
   this should reduce unwanted changes to currentSegment when pressing the switch*/
const int maxThreshold = 1015;

// number of seegments on the display (7 segments + 1 decimal point)
const int segSize = 8;
// segment states 1, 0 (on/off || high/low)
const int segStates = 2;
// matrix for each led on the 7-segment display and it's desired state (initialized with 0)
int segments[segSize][segStates] = {
  {pinDP, statePinDP}, 
  {pinG, statePinG}, 
  {pinF, statePinF}, 
  {pinE, statePinE}, 
  {pinD, statePinD}, 
  {pinC, statePinC},
  {pinB, statePinB},
  {pinA, statePinA}
};
// set constant for (pinSegment - position in matrix)
const int difPinAndIndex = 4;
// initialize the current and starting segment as the decimal point
int currentSegment = pinDP;

// constants used to get either pins or states and not the entire matrix
const int pinsInMatrix = 0;
const int statesInMatrix = 1;

/* the x axis value goes gradually from ~500 to ~1020 so we wait as the jump to interrupt
 is not instant - this should reduce unwanted changes to currentSegment when pressing the switch */
const int swTimeThreshold = 200;

void setup() {
  // initialize joystick's x, y axis and switch input pins
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  // initialize all 7-segments led pins without changing states
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i][pinsInMatrix], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(pinSW), switchInterrupt, CHANGE);
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  //if the joystick was moved after the debouncing time take the input.
  currentTime = millis();
  if(!joyMoved && currentTime - lastJoyMove >= joystickDebounce){
    joyMoved = true;
    lastJoyMove = millis(); 
    currentSegment = changeCurrentSegment();
  }
  // otherwise if the joystick returned in neutral joyMoved should be flagged as such
  else if(xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold){
    joyMoved = false;
  } 
  blink(currentSegment);
  //
  setOtherSegments(currentSegment);
}


// interrupt when the switch is pressed or released
void switchInterrupt() {
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  swState = digitalRead(pinSW);
  /* if interrupts come in faster succession than the debounce delay 
  or if it reads multiple interrupts with the same state, they're ignored */
  if(interruptTime - lastInterruptTime > switchDebounce * 1000 && swState != lastSwState) {
    if(!swState){
      pressTime = millis();
      Serial.print("low");
    }
    if(swState){
      releaseTime = millis();
      Serial.print("  ");
      Serial.println("high");
      if(releaseTime - pressTime < swResetTime){
        Serial.println("segment desired state changed");
        for(int i = 0; i < segSize; i++){
          if(i == currentSegment - difPinAndIndex){
            segments[i][statesInMatrix] = !segments[i][statesInMatrix];
          }
        }
      }
      if(releaseTime - pressTime > swResetTime){
        Serial.println("display reset");
        currentSegment = pinDP;
        for(int i = 0; i < segSize; i++){
          segments[i][statesInMatrix] = 0;
        }
      }
    }
  }
  lastInterruptTime = interruptTime;
  lastSwState = swState;
}

// current segment permanently changes states
void blink(int currentSegment){
  currentBlinkMillis = millis();
  if(currentBlinkMillis - lastBlinkMillis >= blinkingTime){
    state = !state;
    digitalWrite(currentSegment, state);
    lastBlinkMillis = millis();
  }
}

/* takes the current segment and sets all the other ones to the desired
   state (current segment should stay blinking) */
void setOtherSegments(int currentSegment){
  for(int i = 0; i < segSize; i++){
    if(i != currentSegment - difPinAndIndex)
      digitalWrite(segments[i][pinsInMatrix], segments[i][statesInMatrix]); 
  }
}

// when joystick is used, change the current segment according to the table for corresponding movement
int changeCurrentSegment(){
  /* if the switch is pressed even slightly, x axis value goes to max; 
  check if the sw is pressed and wait for the program to jump to to interrupt */
  for(int i = 0; i < swTimeThreshold; i++){
    if(!digitalRead(pinSW)){
      return currentSegment;
    }
  }
  // if both axis are in the set thresholds the joystick was moved diagonally and shouldnt move the segment (doesn't know where to)
  if((xValue > maxThreshold || xValue < minThreshold) && (yValue > maxThreshold || yValue < minThreshold)){
    Serial.println("misinput");
    // return the same segment used to call the function
    return currentSegment;
  }
  switch(currentSegment){
    case pinA:
      if(yValue > maxThreshold){
        currentSegment = pinB;
      }
      if(yValue < minThreshold){
        currentSegment = pinF;
      }
      if(xValue > maxThreshold){
        break;
      }
      if(xValue < minThreshold){
        currentSegment = pinG;
      }
      break;
    case pinB:
      if(yValue > maxThreshold){
        break;
      }
      if(yValue < minThreshold){
        currentSegment = pinF;
      }
      if(xValue > maxThreshold){
        currentSegment = pinA;
      }
      if(xValue < minThreshold){
        currentSegment = pinG;
      }
      break;
    case pinC:
      if(yValue > maxThreshold){
        currentSegment = pinDP;
      }
      if(yValue < minThreshold){
        currentSegment = pinE;
      }
      if(xValue > maxThreshold){
        currentSegment = pinG;
      }
      if(xValue < minThreshold){
        currentSegment = pinD;
      }
      break;
    case pinD:
      if(yValue > maxThreshold){
        currentSegment = pinC;
      }
      if(yValue < minThreshold){
        currentSegment = pinE;
      }
      if(xValue > maxThreshold){
        currentSegment = pinG;
      }
      if(xValue < minThreshold){
        break;
      }
      break;
    case pinE:
      if(yValue > maxThreshold){
        currentSegment = pinC;
      }
      if(yValue < minThreshold){
        break;
      }
      if(xValue > maxThreshold){
        currentSegment = pinG;
      }
      if(xValue < minThreshold){
        currentSegment = pinD;
      }
      break;
    case pinF:
      if(yValue > maxThreshold){
        currentSegment = pinB;
      }
      if(yValue < minThreshold){
        break;
      }
      if(xValue > maxThreshold){
        currentSegment = pinA;
      }
      if(xValue < minThreshold){
        currentSegment = pinG;
      }
      break;
    case pinG:
      if(yValue > maxThreshold){
        break;
      }
      if(yValue < minThreshold){
        break;
      }
      if(xValue > maxThreshold){
        currentSegment = pinA;
      }
      if(xValue < minThreshold){
        currentSegment = pinD;
      }
      break;
    case pinDP:
      if(yValue > maxThreshold){
        break;
      }
      if(yValue < minThreshold){
        currentSegment = pinC;
      }
      if(xValue > maxThreshold){
        break;
      }
      if(xValue < minThreshold){
        break;
      }
      break;
  }
  return currentSegment;
}
