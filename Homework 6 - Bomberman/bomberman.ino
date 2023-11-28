#include "LedControl.h"

// joystick pins
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1; 

const int pinBomb = 3;

// driver/matrix pins
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); 
byte matrixBrightness = 3;
byte matrix[matrixSize][matrixSize] = {
  {1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

// player coordinates
int playerX = 0;
int playerY = 0;

// joystick debounce
const int minThreshold = 200;
const int maxThreshold = 900;
bool joyMoved = false;
const int joystickDebounce = 300;
unsigned long lastJoyMove = 0;
unsigned long currentTime = 0;

// joystick inputs
int xValue = 0;
int yValue = 0;

unsigned long currentSlowBlinkMillis = 0;
unsigned long currentBombBlinkMillis = 0;
unsigned long lastSlowBlinkMillis = 0;
unsigned long lastBombBlinkMillis = 0;
const int slowBlinkingTime = 800;
const int bombBlinkingTime = 100;
byte playerState = 1;
byte wallState = 1;
// time until the bomb 'explodes'
const int bombExplosionTime = 300;
unsigned long bombTEMP = 0;

int maxWalls = 32;
unsigned long lastInterruptTime = 0;
unsigned long lastBombInterruptTime = 0;
unsigned long buttonsDebounce = 300;

byte gameStarted = 0;
long gameStartTime = 0;

// how many walls are left during game
int gameWallCount = 0;

// coordinates for bomb placed
byte bombX = 0;
byte bombY = 0;

void setup() {
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinBomb, INPUT_PULLUP);
  
  EIFR = (1 << INTF1) | (1 << INTF0);

  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  attachInterrupt(digitalPinToInterrupt(pinSW), startGameInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinBomb), placeBombInterrupt, FALLING);
  Serial.begin(9600);
}

void loop() {
  readJoystickValues();
  //if the joystick was moved after the debouncing time take the input.
  joystickMove();
  updateDisplay();
  // player's LED
  playerBlink(playerX, playerY);
  gameFinished();
}

void readJoystickValues(){
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
}

void updateDisplay(){
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
    lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

// handle joystick inputs
void joystickMove(){
  currentTime = millis();
  // check if the joystick was moved beyond thresholds and the debounce time has passed
  if(!joyMoved && currentTime - lastJoyMove >= joystickDebounce && (xValue < minThreshold || xValue > maxThreshold || yValue < minThreshold || yValue > maxThreshold)){
    matrix[playerX][playerY] = 0;
    joyMoved = true;
    lastJoyMove = millis(); 
    playerMovement(playerX, playerY);
    matrix[playerX][playerY] = 1;
  }
    else if(xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold){
    joyMoved = false;
  } 
}

// when joystick was moved, move player position anywhere but not out of display bounds
void playerMovement(int &playerX, int &playerY){
  if(yValue > maxThreshold && playerY < matrixSize - 1 && !matrix[playerX][playerY+1]){
      playerY = playerY + 1;
  }
  if(yValue < minThreshold && playerY > 0 && !matrix[playerX][playerY-1]){
      playerY = playerY - 1;
  }
  if(xValue > maxThreshold && playerX > 0 && !matrix[playerX-1][playerY]){
      playerX = playerX - 1;
    }
  if(xValue < minThreshold && playerX < matrixSize - 1 && !matrix[playerX+1][playerY]){
      playerX = playerX + 1;
  }
}

void playerBlink(int coordX, int coordY){
  currentSlowBlinkMillis = millis();
  if(currentSlowBlinkMillis - lastSlowBlinkMillis >= slowBlinkingTime){
    playerState = !playerState;
    matrix[coordX][coordY] = playerState;
    lastSlowBlinkMillis = millis();
  }
}

void randomizeWalls(){
  //gameStartTime = millis();
  int count = 0;
  long randomNum = -1;
  //int count = 0;
  for (int row = 0; row < matrixSize && count < maxWalls; row++) {
    for (int col = 0; col < matrixSize && count < maxWalls; col++) {
      if((row == 0 && col == 0) || (row == 0 && col == 1) || (row == 1 && col == 0) || (row == 1 && col == 1)){
        matrix[row][col] = 0;
      }
      else{
        randomNum = random(0, 2);
        matrix[row][col] = randomNum;
        if(randomNum){
          count++;
        }
      }
    }
  }
  playerX = 0;
  playerY = 0;
}

void startGameInterrupt() {
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  if((interruptTime - lastInterruptTime > buttonsDebounce * 1000) && !gameStarted){
    randomizeWalls();
    gameStarted = 1;
    lastInterruptTime = interruptTime; 
  }
}

void placeBombInterrupt(){
  static unsigned long bombInterruptTime = 0;
  bombInterruptTime = micros();
  bombTEMP = bombInterruptTime;
  if(bombInterruptTime - lastBombInterruptTime > buttonsDebounce * 1000 && gameStarted == 1){
    // if the value goes beyond matrix borders do not destroy walls on the other side
    playerY + 1 < matrixSize ? matrix[playerX][playerY + 1] = 0 : matrix[playerX][playerY + 1] = matrix[playerX][playerY + 1];
    playerY - 1 >= 0 ? matrix[playerX][playerY - 1] = 0 : matrix[playerX][playerY - 1] = matrix[playerX][playerY - 1];
    playerX + 1 < matrixSize ? matrix[playerX + 1][playerY] = 0 : matrix[playerX + 1][playerY] = matrix[playerX + 1][playerY];
    playerX - 1 >= 0 ? matrix[playerX - 1][playerY] = 0 : matrix[playerX - 1][playerY] = matrix[playerX - 1][playerY];
    playerX + 1 < matrixSize && playerY + 1 < matrixSize ? matrix[playerX + 1][playerY + 1] = 0 : matrix[playerX + 1][playerY + 1] = matrix[playerX + 1][playerY + 1];
    playerX + 1 < matrixSize && playerY - 1 >= 0 ? matrix[playerX + 1][playerY - 1] = 0 : matrix[playerX + 1][playerY - 1] = matrix[playerX + 1][playerY - 1];
    playerX - 1 >= 0 && playerY - 1 >= 0 ? matrix[playerX - 1][playerY - 1] = 0 : matrix[playerX - 1][playerY - 1] = matrix[playerX - 1][playerY - 1]; 
    playerX - 1 >= 0 && playerY + 1 < matrixSize ? matrix[playerX - 1][playerY + 1] = 0 : matrix[playerX - 1][playerY + 1];
    lastBombInterruptTime = millis();
  }
}

// check if there are walls left and set the game as finished if no
void gameFinished(){
  gameWallCount = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if(row != playerX && col != playerY){
        gameWallCount += matrix[row][col];
        if(gameWallCount >= 1){
          return;
        }
      }
    }
  }
  gameStarted = 0;
}

// setting walls that are being destroy to blink really fast | bombX, bombY - coord for bomb | explosion area = 1 block near bomb every way
void bombAreaBlink(int bombX, int bombY){
  currentBombBlinkMillis = millis();
  if(currentBombBlinkMillis - lastBombBlinkMillis >= bombBlinkingTime){
    wallState = !wallState;
    matrix[bombX][bombY] = wallState;
    matrix[bombX][bombY + 1] = wallState;
    matrix[bombX][bombY - 1] = wallState;
    matrix[bombX + 1][bombY] = wallState;
    matrix[bombX - 1][bombY] = wallState;
    matrix[bombX + 1][bombY + 1] = wallState;
    matrix[bombX + 1][bombY - 1] = wallState;
    matrix[bombX - 1][bombY - 1] = wallState;
    matrix[bombX - 1][bombY + 1] = wallState;
    lastBombBlinkMillis = millis();
  }
}