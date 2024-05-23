#include <Servo.h>
#include "Adafruit_VL53L0X.h"

// Define servo objects for each servo
Servo top_gripper;
Servo rotator;
Servo bottom_gripper;
Servo rackPinionServo;

// Define the pin numbers for the servos
const int topGripperPin = 6;
const int rotatorPin = 8;
const int bottomGripperPin = 9;
const int rackPinionServoPin = 10;

// Define motor control pins
const int motorPin1 = 1; // Motor control pin 1
const int motorPin2 = 2; // Motor control pin 2

// Define the switch pin
const int switchPin = 3;

// Define the encoder pins
const int encoderPinA = 0; // Encoder channel A
const int encoderPinB = 10; // Encoder channel B

// Define parameters for rack and pinion movement
const int rackPinionIterations = 10;
const int rackPinionAngle = 5;

// Variables for the encoder counts
volatile long encoderCounts = 0;
long targetCounts = 2000;

// Time of flight sensors
Adafruit_VL53L0X loxTop = Adafruit_VL53L0X();
Adafruit_VL53L0X loxBottom = Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);

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

  // Attach the servo objects to their respective pins
  top_gripper.attach(topGripperPin);
  rotator.attach(rotatorPin);
  bottom_gripper.attach(bottomGripperPin);
  rackPinionServo.attach(rackPinionServoPin);

  // Initialize Time of Flight sensors
  if (!loxTop.begin()) {
    Serial.println(F("Failed to boot top VL53L0X"));
    while (1);
  }
  if (!loxBottom.begin()) {
    Serial.println(F("Failed to boot bottom VL53L0X"));
    while (1);
  }
}

void loop() {
  task3_up();
}

void task3_up() {
  // Ensure both grippers are gripped at the start
  closeServo(1); // Close top gripper
  closeServo(3); // Close bottom gripper

  while (true) {
    openServo(3); // Open bottom gripper
    moveUpUntilSwitchClick();
    moveDownByCounts(100); // Move down by small amount of turns
    closeServo(3); // Close bottom gripper

    moveUpByCounts(200); // Move top gripper up by certain number of turns

    while (true) {
      int distance = getTopToFDistance();
      Serial.print("Top Distance (mm): ");
      Serial.println(distance);

      if (distance < 400) {
        moveDownByCounts(100); // Move down by small amount
        closeServo(1); // Close top gripper
        openServo(3); // Open bottom gripper
        rackPinionForward();
        closeServo(3); // Close bottom gripper
        openServo(1); // Open top gripper
        rackPinionBackward();
        closeServo(1); // Close top gripper
      } else {
        break;
      }
    }

    openServo(1); // Open top gripper
    moveUpByCounts(200); // Move top gripper up
    closeServo(1); // Close top gripper
    openServo(3); // Open bottom gripper
    moveUpByCounts(200); // Move bottom gripper up
  }
}

// Function to move the motor up until the switch clicks, then move it back down by a certain number of turns
void moveUpUntilSwitchClick() {
  startMotorForward();
  while (digitalRead(switchPin) == HIGH) {
    // Wait for the switch to be clicked
  }
  stopMotor();
  moveDownByCounts(200); // Move back down by a certain number of turns
}

// Function to move the motor up by a certain number of counts
void moveUpByCounts(long counts) {
  encoderCounts = 0;
  startMotorForward();
  while (encoderCounts < counts) {
    // Wait for the counts to reach the target
  }
  stopMotor();
}

// Function to move the motor down by a certain number of counts
void moveDownByCounts(long counts) {
  encoderCounts = 0;
  startMotorBackward();
  while (encoderCounts < counts) {
    // Wait for the counts to reach the target
  }
  stopMotor();
}

// Function to turn the servo for rack and pinion in forward direction with multiple small iterations
void rackPinionForward() {
  for (int i = 0; i < rackPinionIterations; i++) {
    rackPinionServo.write(rackPinionServo.read() + rackPinionAngle);
    delay(100);
  }
}

// Function to turn the servo for rack and pinion in reverse direction with multiple small iterations
void rackPinionBackward() {
  for (int i = 0; i < rackPinionIterations; i++) {
    rackPinionServo.write(rackPinionServo.read() - rackPinionAngle);
    delay(100);
  }
}

// Function to open a specific servo
void openServo(int servoNumber) {
  switch (servoNumber) {
    case 1:
      top_gripper.write(180); // Open top gripper
      break;
    case 2:
      rotator.write(180); // Open rotator
      break;
    case 3:
      bottom_gripper.write(180); // Open bottom gripper
      break;
    default:
      break;
  }
}

// Function to close a specific servo
void closeServo(int servoNumber) {
  switch (servoNumber) {
    case 1:
      top_gripper.write(30); // Close top gripper
      break;
    case 2:
      rotator.write(30); // Close rotator
      break;
    case 3:
      bottom_gripper.write(30); // Close bottom gripper
      break;
    default:
      break;
  }
}

// Function to start the motor forward
void startMotorForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}

// Function to start the motor backward
void startMotorBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

// Function to stop the motor
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

// Function to get the distance from the top Time of Flight sensor
int getTopToFDistance() {
  VL53L0X_RangingMeasurementData_t measure;
  loxTop.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {
    return measure.RangeMilliMeter;
  } else {
    return 1000; // Return a large value if out of range
  }
}
