#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>

const int eepromLcdBrightness = 0;
const int eepromMatrixBrightness = 4;

// joystick pins
const int pinSW = 0;
const int pinX = A0;
const int pinY = A1; 

const int pinBomb = 2;

// driver/matrix pins
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const int rs = 9;
const int en = 8;
const int d4 = 7;
const int d5 = 6;
const int d6 = 5;
const int d7 = 4;
const int lcdBrightnessPin = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int intensity = 0;
int intensityMapped = 0; 

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); 
byte matrixBrightness = EEPROM.read(eepromLcdBrightness);
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
unsigned long gameStartTime = 0;

// how many walls are left during game
int gameWallCount = 0;

// coordinates for bomb placed
byte bombX = 0;
byte bombY = 0;

byte displayIsOn = 1;
byte inMenu = 1;
byte isGameFinished = 0;
int pressedSW = 1;
byte inSettings = 0;
byte inBrightnessMatrix = 0;
byte inBrightnessLCD = 0;

unsigned long timer = 0;

void setup() {
  EIFR = (1 << INTF1) | (1 << INTF0);
  
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinBomb, INPUT_PULLUP);
  
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  fullMatrix();

  analogWrite(lcdBrightnessPin, EEPROM.read(eepromLcdBrightness));
  lcd.begin(16, 2);
  
  bootingScreen();

  attachInterrupt(digitalPinToInterrupt(pinBomb), placeBombInterrupt, FALLING);
}

void loop() {
  readJoystickValues();
  //if the joystick was moved after the debouncing time take the input.
  joystickMove();
  updateDisplay();
  if(gameStarted){
    // continiously update game board
    // player's LED
    playerBlink(playerX, playerY);
    // show timer and walls left on lcd
    timer = (millis() - gameStartTime) / 1000;
    lcd.setCursor(0, 1);
    lcd.print(timer);
    if(gameWallCount >= 10){
      lcd.setCursor(14, 1);
      lcd.print(gameWallCount);
    }
    if(gameWallCount < 10){
      lcd.setCursor(14, 1);
      lcd.print("0");
      lcd.print(gameWallCount);
    }
  }
  pressedSW = digitalRead(pinSW);
  if(!pressedSW && isGameFinished){
      fullMatrix();
      restartToMenu();
  }
}

void fullMatrix(){
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, 1);
    }
  }
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
  int count = 0;
  long randomNum = -1;
  //int count = 0;
  for (int row = 0; row < matrixSize && count < maxWalls; row++) {
    for (int col = 0; col < matrixSize && count < maxWalls; col++) {
      if((row == 0 && col == 0) || (row == 0 && col == 1) || (row == 1 && col == 0) || (row == 1 && col == 1)){
        matrix[row][col] = 0;
      }
      else{
        randomNum = random(0, 3);
        if(randomNum){
          matrix[row][col] = 0;
        }
        if(!randomNum){
          count++;
          matrix[row][col] = 2;
        }
      }
    }
  }
  gameWallCount = count;
  playerX = 0;
  playerY = 0;
}

void startGameplay() {
  if(!gameStarted && inMenu){
    randomizeWalls();
    gameStarted = 1;
    isGameFinished = 0;
    inMenu = 0;
    gameStartTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game on!");
  }
}

// check if there are walls left and set the game as finished if no
void gameFinished(){
  gameWallCount = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if(matrix[row][col] == 2){
        gameWallCount++;
      }
    }
  }

  if(!gameWallCount){
    Serial.println("Game finished!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press joystick");
    lcd.setCursor(0, 1);
    lcd.print("for menu.");
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
          matrix[row][col] = 0;
      }
    }
    // smile on matrix
    matrix[5][2] = 1;
    matrix[5][3] = 1;
    matrix[5][4] = 1;
    matrix[5][5] = 1;
    matrix[4][1] = 1;
    matrix[4][6] = 1;
    matrix[1][2] = 1;
    matrix[1][5] = 1;
    matrix[2][2] = 1;
    matrix[2][5] = 1; 
    isGameFinished = 1;
    gameStarted = 0;
  }
}

void placeBombInterrupt(){
  if(inMenu){
    return;
  }
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
    // check walls count after exploding a bomb
    gameFinished();
    lastBombInterruptTime = millis();
  }
}

// setting walls that are being destroy to blink really fast | bombX, bombY - coord for bomb | explosion area = 1 block near bomb every way -- UNUSED 
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

void bootingScreen(){
  const int bootingTime = 10000;
  const int lcdBlinkingInterval = 500; 
  unsigned long currentMillis = millis();
  unsigned long previousMillis = currentMillis;
  Serial.println("Game is booting up!");
  while(currentMillis <= bootingTime){
    if(currentMillis - previousMillis >= lcdBlinkingInterval){
      previousMillis = currentMillis;
      if(displayIsOn){
        lcd.noDisplay();
      }
      else{
        lcd.display();
        lcd.setCursor(3, 0);
        lcd.print("Booting up");
        lcd.setCursor(3, 1);
        lcd.print("the game...");
      }
      displayIsOn = !displayIsOn;
    }
    currentMillis = millis();
  }
  displayIsOn = 1;
  lcd.display();
  lcd.clear();

  displayMenu();
}

void displayMenu(){
  Serial.println("Main Menu:");
  Serial.println("1. Start game");
  Serial.println("2. Settings");
  Serial.println("3. About");
  Serial.println("Enter your choice:");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu displayed");
  lcd.setCursor(0, 1);
  lcd.print("on Serial");
  inMenu = 1;
  inSettings = 0;
  handleMenu();
}

void handleMenu(){
  if(!inMenu){
    return;
  }
  while(!Serial.available()){
    // we wait in the specific menu until a decision was made
  }
  if(Serial.available() > 0){
    int option = Serial.parseInt();

    switch (option) {
      case 1:
        Serial.println("Gameplay started!");
        startGameplay();
        break;
      case 2:
        displaySettingsMenu();  
        handleSettingsMenu();   
        break;
      case 3:
        // Display about section on lcd and on Serial Monitor
        displayAbout();
        break;
      default:
        Serial.println("Invalid option. Please enter an option displayed on the menu.");
        displayMenu();
        break;
    }
  }
}

void displaySettingsMenu(){
  Serial.println("Settings Menu:");
  Serial.println("1. LCD brightness control");
  Serial.println("2. Matrix brightness control");
  Serial.println("3. Back to main menu");
  Serial.println("Enter your option: ");
  inSettings = 1;
  handleSettingsMenu();
}

void handleSettingsMenu(){
  if(!inMenu || !inSettings){
    return;
  }
  while(!Serial.available()){
    // we wait in the specific menu until a decision was made
  }
  if(Serial.available() > 0){
    int option = Serial.parseInt();

    switch (option) {
      case 1:
        Serial.println("Set new LCD brightness (0 - 5): ");
        inBrightnessLCD = 1;
        handleBrightness();
        break;
      case 2:
        Serial.println("Set new Matrix brightness (0 - 5): ");
        inBrightnessMatrix = 1;
        handleBrightness();
        break;
      case 3:
        // Display about section on lcd and on Serial Monitor
        Serial.println("Back to main menu.");
        displayMenu();
        break;
      default:
        Serial.println("Invalid option. Please enter an option displayed on the menu.");
        displaySettingsMenu();
        break;
    }
  }
}

void handleBrightness(){
  while(!Serial.available()){
    // we wait in the specific menu until a decision was made
  }
  if(Serial.available() > 0){
    intensity = Serial.parseInt();
    Serial.println(intensity);
    if(intensity <= 5 && intensity >= 0){
      if(inBrightnessLCD){
        intensityMapped = map(intensity, 0, 5, 0, 255);
        EEPROM.update(eepromLcdBrightness, intensityMapped);
        analogWrite(lcdBrightnessPin, EEPROM.read(eepromLcdBrightness));
        inBrightnessLCD = 0;
      }
      else if(inBrightnessMatrix){
        intensityMapped = map(intensity, 0, 5, 0, 15);
        EEPROM.update(eepromMatrixBrightness, intensityMapped);
        lc.setIntensity(0, intensityMapped);
        inBrightnessMatrix = 0;
      }
      inSettings = 0;
      displayMenu();
    }
    else{
      Serial.println("Invalid input.");
      Serial.println("Enter new brightness: ");
      handleBrightness();
    }
  }
}

void displayAbout(){
  if(!inMenu){
    return;
  }
  Serial.println("School project:");
  Serial.println("Matrix bomberman game");
  Serial.println("5 minutes, how many levels can you clear?");
  Serial.println("- Alex");
  // to add lcd displayed text
  displayMenu();
}

void restartToMenu(){
  if(inMenu == 0 && isGameFinished == 1){
    inMenu = 1;
    displayMenu();
  }
}
