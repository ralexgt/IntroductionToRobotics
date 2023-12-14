# **Introduction To Robotics (2023 - 2024)**

_Introduction to Robotics - Faculty of Mathematics and Computer Science, University of Bucharest - 3rd year | 1st semester_ </br> </br>
Repository for laboratory homeworks - including requirements, files, implementation, source code, setup images and/or videos.

## [Homework #1](#hw1) <a name="hw1"></a>

- ### **_Description_**

  This assigment focuses on controlling each channel of an RGB LED using (3) individual potentiometeres.
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    Use a separate potentiometer for controlling each color of the RGB LED: Red, Green and Blue. This control must leverage digital electronics. Specifically, you need to read the potentiometer’s value with Arduino and then write a mapped value to the LED pins.
  - **_Components_** </br>
    - RGB LED (at least 1);
    - Potentiometers (at least 3);
    - Resistors and wires as needed;
      </br></br>

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%201%20-%20RGB%20LED"> Homework 1 - RGB LED </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 1 - RGB LED/rgbSetup.jpg" width="500" height="300">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://youtu.be/50T870-A4no"> Introduction to Robotics - Homework 1 (RGB LED) [YouTube] </a>

</br></br>

## [Homework #2 - WIP](#hw2) <a name="hw2"></a>

- ### **_Description_**

  This assignment involves simulating a 3-floor elevator control system using
  LEDs, buttons, and a buzzer with Arduino.
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    Design a control system that simulates a 3-floor elevator using the Arduino platform.
    </br></br>
    Each of the 3 LEDs should represent one of the 3 floors.
    The LED corresponding to the current floor should light up. Additionally,
    another LED should represent the elevator’s operational state. It should
    blink when the elevator is moving and remain static when stationary.
    </br></br>
    Implement 3 buttons that represent the call buttons from the
    3 floors. When pressed, the elevator should simulate movement towards
    the floor after a short interval (2-3 seconds).
    </br></br>
    The buzzer should sound briefly when arriving at the desired floor and the elevator doors are closing or the elevator is moving.
    </br></br>
    If the elevator is already at the desired floor,
    pressing the button for that floor should have no effect. Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the corresponding floor. If the elevator is in movement, it should either do nothing or it should stack its decision.
    </br></br>
    Remember to implement debounce for the buttons to avoid
    unintentional repeated button presses.

  - **_Components_** </br>
    - RGB LED (at least 4 - 1 for elevator state and 3 for each floor);
    - Buttons (at least 3, 1 for each floor);
    - Resistors and wires as needed;
      </br></br>

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%202%20-%20Elevator%20Simulator%20-%20WIP"> Homework 2 - Elevator Simulator WIP </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 2 - Elevator Simulator - WIP/elevatorSetup.jpg" width="600" height="400">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://github.com/ralexgt/IntroductionToRobotics"> TBA </a>

</br></br>

## [Homework #3](#hw3) <a name="hw3"></a>

- ### **_Description_**

  We will use the joystick to control the position of
  the segment and ”draw” on the display. The movement between segments
  will be natural, meaning they should jump from the current position
  only to neighbors, but without passing through ”walls”.
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    The initial position should be on the decimal point. The current
    position always blinks (ignoring the state of the led). Use the joystick to move from one position to neighbouring leds. Short pressing the button toggles the segment state from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
  - **_Components_** </br>
    - 7-segment display;
    - joystick;
    - Resistors and wires as needed;

- **_Table of neighbours_** </br>

| Current segment         | UP  | DOWN | LEFT | RIGHT |
| ----------------------- | :-- | ---: | ---: | ----: |
| &emsp; &emsp; &emsp; a  | N/A |    g |    f |     b |
| &emsp; &emsp; &emsp; b  | a   |    g |    f |   N/A |
| &emsp; &emsp; &emsp; c  | g   |    d |    e |    dp |
| &emsp; &emsp; &emsp; d  | g   |  N/A |    e |     c |
| &emsp; &emsp; &emsp; e  | g   |    d |  N/A |     c |
| &emsp; &emsp; &emsp; f  | a   |    g |  N/A |     b |
| &emsp; &emsp; &emsp; g  | a   |    d |  N/A |   N/A |
| &emsp; &emsp; &emsp; dp | N/A |  N/A |    c |   N/A |

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%203%20-%207segment%20drawing"> Homework 3 - 7segment drawing </a>
    </br></br>

- ### **_Setup picture_**

  <img title="tba" src="./Homework 3 - 7segment drawing/hw3-intro-rob-setup.jpg" width="600" height="400">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://youtu.be/5F7jvzxB36o"> Introduction to Robotics - Homework 3 (7segment drawing) [YouTube] </a>

</br></br>

## [Homework #4](#hw4) <a name="hw4"></a>

- ### **_Description_**

  We will implement a stopwatch timer that counts in 10ths of a second
  and has a save lap functionality using the 4 digits 7-segment display and 3 buttons.
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    The starting value of the 4 digits 7-segment display should
    be ”000.0” and the buttons should have the following functionalities:

    - Button 1: Start / pause.
    - Button 2: Reset (if in pause mode). Reset saved laps (if in lap viewing mode).
    - Button 3: Save lap (if in counting mode), cycle through last saved laps (up to 4 laps).

  - **_Components_** </br>
    - 1 4 digits 7-segment display;
    - 3 buttons;
    - Resistors and wires as needed;
      </br></br>

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%204%20-%20Stopwatch"> Homework 4 - Stopwatch </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 4 - Stopwatch/Intro To Robotics - HW5 - Stopwatch1.jpg" width="500" height="400">

  </br></br>

- ### **_Functionality video:_**
  <a href="https://youtu.be/cIefdwOfJhA"> Introduction to Robotics - Homework 4 (Stopwatch) [YouTube] </a>

</br></br>

## [Homework #6](#hw6) <a name="hw6"></a>

- ### **_Credits_**
  "EIFR = (1 << INTF1) | (1 << INTF0);" - directly from https://forum.arduino.cc/t/left-over-variables-on-reset-or-strange-interrupt-behaviour/328693/9 - only first interrupt worked properly before
- ### **_Description_**

  This assigment is a 8x8 matrix game! The game is composed of a player (blinking LED), walls (static LED's) and bombs. The joystick switch is a 'start game' button,
  the button is used to place bombs and the joystick to move the player around. You can not move outside of the matrix boundries and you can not pass through walls.
  All walls are destructable and the bomb has an area of effect of 1 LED in each direction. The game is concluded when you destroy all walls and you can restart the
  game with the joystick switch.
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    Develop a small game on the 8x8 matrix. The game must have at least 3 types
    of elements: player (blinks slowly), bombs/bullets (blinks fast), wall (doesn’t
    blink). The basic idea is that you generate walls on the map and then
    you move around with the player and destroy them.

  - **_Components_** </br>
    - Arduino Uno Board;
    - Joystick;
    - 8x8 LED Matrix;
    - MAX7219;
    - Resistors and capacitors as needed;
    - Breadboard and connecting wires;
      </br></br>

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%206%20-%20Bomberman"> Homework 6 - Bomberman </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 6 - Bomberman/bombermanSetup.jpg" width="500" height="300">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://youtu.be/2MiOdwYz2tk"> Introduction to Robotics - Homework 6 (Matrix game) [YouTube] </a>

</br></br>

## [Homework #7](#hw7) <a name="hw7"></a>

- ### **_Description_**

  This is the first checkpoint for our Matrix Game Project. I implemented a starting screen, gameplay starter, an end game and a menu to navigate between the game, game's settings and some info about the game, as well as some information during the game, displayed on the LCD (such as time played, walls left).
  </br></br>

- ### **_Requirements_**

  - **_Technical task_** </br>
    Add the following functionality for this checkpoint:

    1. Intro message
    2. Menu
    3. End message
    4. Gameplay details / end game screen

  - **_Menu_** </br>
    - Start game
    - Settings
      - LCD brightness control
      - Matrix brightness control
      - Back to main menu
    - About
      </br></br>

- ### **_Files_**

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%207%20-%20Matrix%20checkpoint"> Homework 7 - Matrix project checkpoint 1 </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 7 - Matrix checkpoint/setup_photo.jpg" width="500" height="300">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://youtu.be/jUd10wDJPGw"> Introduction to Robotics - Homework 7 (Matrix project cp 1) [YouTube] </a>

</br></br>
