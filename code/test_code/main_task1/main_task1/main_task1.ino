#include <Servo.h>

// Define servo objects
Servo top_gripper;
Servo bottom_gripper;

// Define the pin numbers for the servos
const int topGripperPin = 6;
const int bottomGripperPin = 9;

// Define the positions for open and close
const int openPosition = 180; // Adjust as needed for your servo
const int closePosition = 30; // Adjust as needed for your servo

// Define motor control pins
const int motorPin1 = 1; // Motor control pin 1
const int motorPin2 = 2; // Motor control pin 2

// Define the switch pin
const int switchPin = 3;

// Define the encoder pins
const int encoderPinA = 0; // Encoder channel A
const int encoderPinB = 10; // Encoder channel B

// Variables for the encoder counts
volatile long encoderCounts = 0;
const long targetCounts = 2000; // Number of counts to move down after the switch is clicked

void setup() {
  // Attach the servo objects to their respective pins
  top_gripper.attach(topGripperPin);
  bottom_gripper.attach(bottomGripperPin);

  // Initialize motor control pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Initialize switch pin with internal pull-up resistor
  pinMode(switchPin, INPUT_PULLUP);

  // Initialize encoder pins
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  // Attach interrupt service routines to encoder pins
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderAChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), encoderBChange, CHANGE);

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Open bottom gripper
  openBottomGripper();
  delay(1000); // Wait for the gripper to open

  // Move the bottom arm up until the switch clicks, then move it down by a certain number of turns
  moveBottomArmUpUntilSwitch();
  moveBottomArmDownByCounts(targetCounts);

  // Close bottom gripper
  closeBottomGripper();
  delay(1000); // Wait for the gripper to close

  // Open top gripper
  openTopGripper();
  delay(1000); // Wait for the gripper to open

  // Move the top arm up by a certain number of turns
  moveTopArmUpByCounts(targetCounts);

  // Close top gripper
  closeTopGripper();
  delay(1000); // Wait for the gripper to close
}

// Function to open bottom gripper
void openBottomGripper() {
  bottom_gripper.write(openPosition);
}

// Function to close bottom gripper
void closeBottomGripper() {
  bottom_gripper.write(closePosition);
}

// Function to open top gripper
void openTopGripper() {
  top_gripper.write(openPosition);
}

// Function to close top gripper
void closeTopGripper() {
  top_gripper.write(closePosition);
}

// Function to move the bottom arm up until the switch is clicked
void moveBottomArmUpUntilSwitch() {
  startMotorForward();
  while (digitalRead(switchPin) == HIGH) {
    // Do nothing and wait for the switch to be pressed
  }
  stopMotor();
}

// Function to move the bottom arm down by a certain number of encoder counts
// void moveBottomArmDownByCounts(long counts) {
//   encoderCounts = 0;
//   startMotorBackward();
//   while (encoderCounts < counts) {
//     // Do nothing and wait for the counts to reach the target
//   }
//   stopMotor();
// }

// Function to move the top arm up by a certain number of encoder counts
// void moveTopArmUpByCounts(long counts) {
//   encoderCounts = 0;
//   startMotorForward();
//   while (encoderCounts < counts) {
//     // Do nothing and wait for the counts to reach the target
//   }
//   stopMotor();
// }

// Motor control functions
void startMotorForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

void startMotorBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

// // Encoder interrupt service routines
// void encoderAChange() {
//   if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
//     encoderCounts++;
//   } else {
//     encoderCounts--;
//   }
// }

// void encoderBChange() {
//   if (digitalRead(encoderPinA) != digitalRead(encoderPinB)) {
//     encoderCounts++;
//   } else {
//     encoderCounts--;
//   }
// }
