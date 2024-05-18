#include <Servo.h>

// Define servo objects for each servo
Servo servo1;
Servo servo2;
Servo servo3;

// Define pin numbers for each servo
const int servoPin1 = 6;
const int servoPin2 = 8;
const int servoPin3 = 9;

void setup() {
  // Attach each servo to its corresponding pin
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
}

void loop() {
  // Move each servo to a different angle
  moveServo(servo1, 0);   // Move servo1 to 0 degrees
  moveServo(servo2, 90);  // Move servo2 to 90 degrees
  moveServo(servo3, 180); // Move servo3 to 180 degrees

  delay(1000); // Delay for 1 second before next movement
  
  moveServo(servo1, 45);   // Move servo1 to 0 degrees
  moveServo(servo2, 30);  // Move servo2 to 90 degrees
  moveServo(servo3, 105); // Move servo3 to 180 degrees

  delay(1000); // Delay for 1 second before next movement
  
}

// Function to move a servo to a specific angle
void moveServo(Servo servo, int angle) {
  servo.write(angle);
  delay(500); // Delay for stability, adjust as needed
}
