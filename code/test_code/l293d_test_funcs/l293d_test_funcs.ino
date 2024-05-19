const int IN1 = 1;
const int IN2 = 2;

void setup() {
  // Initialize the control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  // Ensure the motor is stopped initially
  linearStop();
}

void linearUp() {
  // Set IN1 high and IN2 low to rotate the motor in one direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void linearDown() {
  // Set IN1 low and IN2 high to rotate the motor in the opposite direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void linearStop() {
  // Set both IN1 and IN2 low to stop the motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  // Example usage: Move up for 2 seconds, down for 2 seconds, and then stop
//  linearUp();
//  delay(2000);
//  linearStop();
//  delay(500);
  linearDown();
//  delay(2000);
//  linearStop();
//  delay(500);
}
