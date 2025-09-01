// Motor ON/OFF using Relay and Arduino
// Relay is controlled by a push button

int relayPin = 7;      // Relay input pin connected to Arduino pin 7
int buttonPin = 2;     // Push button input pin
int buttonState = 0;   // Variable to store button status

void setup() {
  pinMode(relayPin, OUTPUT);    // Relay pin as output
  pinMode(buttonPin, INPUT);    // Button pin as input
}

void loop() {
  buttonState = digitalRead(buttonPin);  // Read button state

  if (buttonState == HIGH) {
    // If button is pressed → turn ON motor
    digitalWrite(relayPin, HIGH);   // Relay ON → Motor ON
  } else {
    // If button not pressed → turn OFF motor
    digitalWrite(relayPin, LOW);    // Relay OFF → Motor OFF
  }
}
