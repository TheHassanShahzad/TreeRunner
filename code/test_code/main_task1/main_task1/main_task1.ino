#include <Servo.h>
#include "Adafruit_VL53L0X.h"

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
const long targetCounts = 2000; // Number of counts to move up or down

// Time of Flight sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const int distanceThreshold = 50; // Distance threshold in mm to detect the canopy

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

  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize the Time of Flight sensor
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void loop() {
  // Read the Time of Flight sensor
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  // Print the distance to the serial monitor
  if (measure.RangeStatus != 4) {
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }

  // Check if the distance is very small, indicating the canopy is reached
  if (measure.RangeMilliMeter < distanceThreshold) {
    // Start the sequence to climb down
    climbDownSequence();
  } else {
    // Start the sequence to climb up
    climbUpSequence();
  }

  delay(100); // Small delay to prevent excessive serial output
}

// Function to climb up
void climbUpSequence() {
  openBottomGripper();
  delay(1000); // Wait for the gripper to open

  moveBottomArmUpUntilSwitch();
  moveBottomArmDownByCounts(targetCounts);

  closeBottomGripper();
  delay(1000); // Wait for the gripper to close

  openTopGripper();
  delay(1000); // Wait for the gripper to open

  moveTopArmUpByCounts(targetCounts);

  closeTopGripper();
  delay(1000); // Wait for the gripper to close
}

// Function to climb down
void climbDownSequence() {
  openTopGripper();
  delay(1000); // Wait for the gripper to open

  moveTopArmDownUntilSwitch();
  moveTopArmUpByCounts(targetCounts);

  closeTopGripper();
  delay(1000); // Wait for the gripper to close

  openBottomGripper();
  delay(1000); // Wait for the gripper to open

  moveBottomArmDownByCounts(targetCounts);

  closeBottomGripper();
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

// Function to move the top arm down until the switch is clicked
void moveTopArmDownUntilSwitch() {
  startMotorBackward();
  while (digitalRead(switchPin) == HIGH) {
    // Do nothing and wait for the switch to be pressed
  }
  stopMotor();
}

// Function to move the top arm up by a certain number of encoder counts
void moveTopArmUpByCounts(long counts) {
  encoderCounts = 0;
  startMotorForward();
  while (encoderCounts < counts) {
    // Do nothing and wait for the counts to reach the target
  }
  stopMotor();
}

// Function to move the bottom arm down by a certain number of encoder counts
void moveBottomArmDownByCounts(long counts) {
  encoderCounts = 0;
  startMotorBackward();
  while (encoderCounts < counts) {
    // Do nothing and wait for the counts to reach the target
  }
  stopMotor();
}

// Function to move the bottom arm up until the switch is clicked
void moveBottomArmUpUntilSwitch() {
  startMotorForward();
  while (digitalRead(switchPin) == HIGH) {
    // Do nothing and wait for the switch to be pressed
  }
  stopMotor();
}

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

// Encoder interrupt service routines
void encoderAChange() {
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    encoderCounts++;
  } else {
    encoderCounts--;
  }
}

void encoderBChange() {
  if (digitalRead(encoderPinA) != digitalRead(encoderPinB)) {
    encoderCounts++;
  } else {
    encoderCounts--;
  }
}
