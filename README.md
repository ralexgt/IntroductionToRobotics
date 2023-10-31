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

  - <a href="https://github.com/ralexgt/IntroductionToRobotics/tree/main/Homework%202%20-%20Elevator%20Simulator"> Homework 2 - Elevator Simulator </a>
    </br></br>

- ### **_Setup picture_**

  <img src="./Homework 2 - Elevator Simulator/elevatorSetup.jpg" width="600" height="400">
  </br></br>

- ### **_Functionality video:_**
  <a href="https://github.com/ralexgt/IntroductionToRobotics"> TBA </a>
