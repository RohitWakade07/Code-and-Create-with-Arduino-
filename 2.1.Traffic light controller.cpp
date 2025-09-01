// Traffic Light Controller using Arduino
// Controls 3 LEDs (Red, Yellow, Green) to simulate a traffic signal

// Define LED pins (connected to Arduino digital pins)
int red = 2;      // Red LED connected to pin 2
int yellow = 3;   // Yellow LED connected to pin 3
int green = 4;    // Green LED connected to pin 4

void setup() {
  // Configure LED pins as OUTPUT
  pinMode(red, OUTPUT);     
  pinMode(yellow, OUTPUT);  
  pinMode(green, OUTPUT);   
}

void loop() {
  // Step 1: Turn ON RED light → Stop signal
  digitalWrite(red, HIGH);     // Red ON
  digitalWrite(yellow, LOW);   // Yellow OFF
  digitalWrite(green, LOW);    // Green OFF
  delay(5000);                 // Hold for 5 seconds

  // Step 2: Turn ON YELLOW light → Get ready
  digitalWrite(red, LOW);      // Red OFF
  digitalWrite(yellow, HIGH);  // Yellow ON
  digitalWrite(green, LOW);    // Green OFF
  delay(2000);                 // Hold for 2 seconds

  // Step 3: Turn ON GREEN light → Go signal
  digitalWrite(red, LOW);      // Red OFF
  digitalWrite(yellow, LOW);   // Yellow OFF
  digitalWrite(green, HIGH);   // Green ON
  delay(5000);                 // Hold for 5 seconds

  // After this, loop() restarts automatically and repeats cycle
}
