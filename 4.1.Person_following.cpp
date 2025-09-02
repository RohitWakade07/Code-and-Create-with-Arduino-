// ================= MOTOR DRIVER (L298N) PIN DEFINITIONS =================
#define ENA 5    // Enable pin for Motor A (controls speed via PWM)
#define IN1 6    // Input pin 1 for Motor A (direction)
#define IN2 7    // Input pin 2 for Motor A (direction)

#define ENB 9    // Enable pin for Motor B (controls speed via PWM)
#define IN3 10   // Input pin 1 for Motor B (direction)
#define IN4 11   // Input pin 2 for Motor B (direction)

// ================= ULTRASONIC SENSOR (HC-SR04) PIN DEFINITIONS =================
#define TRIG 3   // Ultrasonic Trigger pin (sends signal)
#define ECHO 2   // Ultrasonic Echo pin (receives signal)

// Variables for distance measurement
long duration;   // Time taken for ultrasonic pulse to return
int distance;    // Calculated distance in centimeters

// ================= SETUP FUNCTION =================
void setup() {
  Serial.begin(9600);  // Start serial monitor at 9600 baud rate for debugging

  // Set motor driver pins as OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(TRIG, OUTPUT);  // Trigger pin sends signal
  pinMode(ECHO, INPUT);   // Echo pin receives signal

  // Set initial motor speed using PWM (0–255)
  analogWrite(ENA, 150);  // Motor A speed (about 60% power)
  analogWrite(ENB, 150);  // Motor B speed (about 60% power)
}

// ================= FUNCTION TO GET DISTANCE USING ULTRASONIC =================
int getDistance() {
  digitalWrite(TRIG, LOW);          // Make sure trigger is low
  delayMicroseconds(2);             // Wait 2 microseconds

  digitalWrite(TRIG, HIGH);         // Send 10 microsecond HIGH pulse
  delayMicroseconds(10);            // This starts ultrasonic burst
  digitalWrite(TRIG, LOW);          // Set trigger low again

  duration = pulseIn(ECHO, HIGH);   // Measure how long echo pin stayed HIGH
  distance = duration * 0.034 / 2;  // Convert time to distance in cm
  return distance;                  // Return the measured distance
}

// ================= FUNCTION TO MOVE BOT FORWARD =================
void moveForward() {
  // Motor A rotates forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor B rotates forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// ================= FUNCTION TO STOP MOTORS =================
void stopMotors() {
  // Motor A stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Motor B stop
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ================= MAIN LOOP =================
void loop() {
  int d = getDistance();   // Get the current distance
  Serial.println(d);       // Print distance to serial monitor for debugging

  if (d > 20 && d < 100) { // If distance is between 20 cm and 100 cm
    moveForward();         // Move forward (follow the person)
  } else {                 
    stopMotors();          // Otherwise stop (too close or too far)
  }

  delay(50);               // Small delay to stabilize readings
}
