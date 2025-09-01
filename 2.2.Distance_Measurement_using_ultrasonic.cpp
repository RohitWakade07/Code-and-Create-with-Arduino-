// Distance Measurement using Ultrasonic Sensor (HC-SR04)
// The sensor measures distance by sending sound waves and receiving echo.

// Define pins for HC-SR04
const int trigPin = 9;   // Trigger pin connected to Arduino pin 9
const int echoPin = 10;  // Echo pin connected to Arduino pin 10

// Variable to store duration and distance
long duration;   // Time taken for echo to return
int distance;    // Calculated distance in cm

void setup() {
  pinMode(trigPin, OUTPUT);  // Trigger pin as output
  pinMode(echoPin, INPUT);   // Echo pin as input
  Serial.begin(9600);        // Start serial communication to display results
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10µs HIGH pulse to trigger the sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time taken for the echo to return
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance (in cm)
  // Speed of sound = 343 m/s = 0.034 cm/µs
  // Distance = (Time × Speed of Sound) / 2
  distance = duration * 0.034 / 2;

  // Print distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Wait half a second before next reading
}
