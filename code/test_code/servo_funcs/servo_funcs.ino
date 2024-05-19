#include <Servo.h>

// Create servo objects
Servo top_gripper;
Servo rotator;
Servo bottom_gripper;

// Define the pin numbers for the servos
const int topGripperPin = 6;
const int rotatorPin = 8;
const int bottomGripperPin = 9;

// Define the positions for open and close
const int openPosition = 180; // Adjust as needed for your servo
const int closePosition = 30;  // Adjust as needed for your servo
const int loosenPosition = 60; // Adjust as needed for your application

void setup() {
  // Attach the servo objects to their respective pins
  top_gripper.attach(topGripperPin);
  rotator.attach(rotatorPin);
  bottom_gripper.attach(bottomGripperPin);
}

// Function to open a specific servo
void openServo(int servoNumber) {
  switch (servoNumber) {
    case 1:
      top_gripper.write(openPosition);
      break;
    case 2:
      rotator.write(openPosition);
      break;
    case 3:
      bottom_gripper.write(openPosition);
      break;
    default:
      // Invalid servo number
      break;
  }
}

// Function to close a specific servo
void closeServo(int servoNumber) {
  switch (servoNumber) {
    case 1:
      top_gripper.write(closePosition);
      break;
    case 2:
      rotator.write(closePosition);
      break;
    case 3:
      bottom_gripper.write(closePosition);
      break;
    default:
      // Invalid servo number
      break;
  }
}

// Function to loosen a specific servo to the loosenAngle
void loosenServo(int servoNumber) {
  switch (servoNumber) {
    case 1:
      top_gripper.write(loosenPosition);
      break;
    case 2:
      rotator.write(loosenPosition);
      break;
    case 3:
      bottom_gripper.write(loosenPosition);
      break;
    default:
      // Invalid servo number
      break;
  }
}

void loop() {
  // Example usage of the functions
  openServo(1); // Open top_gripper
//  delay(1000); // Wait for 1 second
//  
//  openServo(2); // Open rotator
//  delay(1000); // Wait for 1 second
//  
//  openServo(3); // Open bottom_gripper
//  delay(1000); // Wait for 1 second
//
//  closeServo(1); // Close top_gripper
//  delay(1000); // Wait for 1 second
//  
//  closeServo(2); // Close rotator
//  delay(1000); // Wait for 1 second
//  
//  closeServo(3); // Close bottom_gripper
//  delay(1000); // Wait for 1 second
//  
//  loosenServo(1); // Loosen top_gripper
//  delay(1000); // Wait for 1 second
//  
//  loosenServo(2); // Loosen rotator
//  delay(1000); // Wait for 1 second
//  
//  loosenServo(3); // Loosen bottom_gripper
//  delay(1000); // Wait for 1 second
}
