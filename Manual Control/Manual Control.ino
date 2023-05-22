/* This code creates a feedback control system that reads analog sensor 
values, processes them, and controls an actuator based on the input value. 
Additionally, it aims to provide real-time feedback by displaying sensor 
readings, control outputs, and input values on an LCD screen.*/

#include <LiquidCrystal.h>

// Define the pins for LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin for controlling LCD backlight
int backlightPin = 9;

// Initial contrast value for the LCD
int contrast = 70;

// Analog pins for sensors and actuators
int sensor = A1;     // Setting the pin for the sensor 
int inputPin = A5;    // Setting the pin for the potentiometer which will be the SP
int actuator = 10;   // Select the pin for the LED

// Variables for storing sensor readings and control values
int PV = 0;          // Sensor reading / LDR value
int inputValue = 0;  // Input value from A5 pin/ potentiometer
int CO = 0;          // Control output

void setup() {
  // Set the backlight pin as an output
  pinMode(backlightPin, OUTPUT);

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);

  // Set up the actuator pin as an output
  pinMode(actuator, OUTPUT);

  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {
  // Set the backlight brightness using the contrast value
  analogWrite(backlightPin, contrast);

  // Read the sensor value between 0-1024
  PV = analogRead(sensor);

  // Read the input value from A5 pin between 0-1024
  inputValue = analogRead(inputPin);

  // Map the input value to the range 0-255 for control output
  CO = map(inputValue, 15, 1000, 0, 255);

  // Set the actuator output using the control output value
  analogWrite(actuator, CO);

  // Print sensor readings and control output values to serial monitor
  Serial.print("\t PV = ");
  Serial.print(PV);
  Serial.print("\t input = ");
  Serial.print(inputValue);
  Serial.print("\t CO = ");
  Serial.print(CO);
  Serial.print("\t PV = ");
  Serial.print(PV);
  Serial.println();

  // Clear the LCD display
  lcd.clear();

  // Print the sensor value (PV) on the LCD
  lcd.setCursor(0, 0);
  lcd.print("PV: ");
  lcd.print(PV);

  // Print the control output value (CO) on the LCD
  lcd.setCursor(8, 0);
  lcd.print("CO: ");
  lcd.print(CO);

  // Print the input value on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Input: ");
  lcd.print(inputValue);
}
