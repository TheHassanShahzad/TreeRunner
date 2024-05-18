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
long targetCounts = 2000;

void setup() {
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

  // Start the motor
  startMotorForward();
}

void loop() {
  // Check if the switch is pressed
  if (digitalRead(switchPin) == LOW) {
    // Stop the motor
    stopMotor();

    // Wait a short while to debounce the switch
    delay(50);

    // Reset the encoder counts
    encoderCounts = 0;

    // Reverse the motor direction
    startMotorBackward();

    // Run the motor until the encoder counts reach the target
    while (encoderCounts < targetCounts) {
      // Do nothing and wait for the counts to reach the target
    }

    // Stop the motor
    stopMotor();
  }
}

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
