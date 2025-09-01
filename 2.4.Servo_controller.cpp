// Controlling Servo Motor with Potentiometer
// Servo angle will change as you rotate the potentiometer

#include <Servo.h>   // Include Servo library

Servo myServo;       // Create a Servo object

int potPin = A0;     // Potentiometer connected to analog pin A0
int potValue;        // Variable to store potentiometer reading
int angle;           // Variable to store servo angle

void setup() {
  myServo.attach(9);  // Attach servo signal wire to pin 9
}

void loop() {
  // Read potentiometer value (0 to 1023)
  potValue = analogRead(potPin);

  // Map the potentiometer value to servo angle (0° to 180°)
  angle = map(potValue, 0, 1023, 0, 180);

  // Move servo to the new angle
  myServo.write(angle);

  delay(15);  // Small delay for smooth movement
}
