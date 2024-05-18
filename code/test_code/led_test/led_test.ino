// Blink Sketch

// Define the pin number for the LED
const int ledPin = 13;

// Setup function runs once when you press reset or power the board
void setup() {
  // Initialize the digital pin as an output
  pinMode(ledPin, OUTPUT);
}

// Loop function runs over and over again forever
void loop() {
  // Turn the LED on (HIGH is the voltage level)
  digitalWrite(ledPin, HIGH);
  // Wait for 1 second
  delay(1000);
  // Turn the LED off by making the voltage LOW
  digitalWrite(ledPin, LOW);
  // Wait for 1 second
  delay(1000);
}
