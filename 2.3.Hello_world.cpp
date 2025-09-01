// Display "Hello World" on 16x2 LCD with I2C Module

#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD with I2C

// Create LCD object (I2C address: 0x27 or 0x3F depending on module)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();            // Initialize LCD
  lcd.backlight();       // Turn on LCD backlight

  // Print message on LCD
  lcd.setCursor(0, 0);   // Set cursor to column 0, row 0
  lcd.print("Hello World!");  // Print first line

  lcd.setCursor(0, 1);   // Set cursor to column 0, row 1
  lcd.print("Arduino LCD");   // Print second line
}

void loop() {
  // Nothing needed here, text stays on LCD
}
