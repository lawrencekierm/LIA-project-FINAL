/*This code is creates a control system that uses an analog sensor to control an actuator. 
The code also incorporates a user interface through buttons and an LCD display to allow 
the user to adjust the setpoint and hysteresis values of the control system. Additionally,
all the information in values that includes PV, SP, CO and HYS are displayed on the LCD. The circuit 
is also built in 4 buttons, which the first 2 allow the user to switch screens on the LCD,
and the other 2 allow the user to increase or decrease the value in HYS and SP. */

#include <LiquidCrystal.h>

// Pin for the buttons
const int buttonsPin = A4;  // All four buttons are connected to one analogInput ( A4)
int buttonsValue = 0;  // declare button value that ranges from 0-1024
int screenState = 0;  // declare screen state

// Variables for setpoint and hysteresis
int setPoint = 0;  
int HYSplus;  // declare added value for HYS
int HYSminus;  // declare subtracted value for HYS 
int setPointplus;  // declare added value for SP
int setPointminus;  // declare subtracted value for SP

// Analog pins for sensors and actuators
int sensor = A1;  // Setting the pin for the sensor
int actuator = 10;   // Select the pin for the LED
int PV = 0;         //  The process variable / LDR value
int CO = 0;         // Control output value
int HYS = 30;       // Hysteresis Value


// LCD setup
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int backlightPin = 9;  // Pin for controlling LCD backlight
int contrast = 70;    // Initial contrast value for the LCD


void setup() {
  // Set the backlight pin as an output
  pinMode(backlightPin, OUTPUT);

  // Set up the actuator pin as an output
  pinMode(actuator, OUTPUT);

  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
}



void loop() {

  // Set the backlight brightness using the contrast value
  analogWrite(backlightPin, contrast);

  // Read the analog sensor value (PV) between 0-1024
  PV = analogRead(sensor);

  // Determine the control output (CO) based on the setpoint and hysteresis
  if (PV <= setPoint - HYS)
  {
    CO = 255;        // using the PWM output as digital so 255 represents 5V which is logic 1 (HIGH)
    
  }
  else if (PV >= setPoint + HYS)
  { 
    CO = 0;   // Logic 0 (LOW)
    
  }

// Set the actuator output using the control output value
analogWrite(actuator, CO);

// Handle button presses and update setpoint or hysteresis
if(screenState == 3) {
  // If button 4 is pressed on screen 3, increase the value in HYS by 5
  if (buttonsValue < 800 && buttonsValue > 750) {
    HYSplus = HYS + 5;
    HYS = HYSplus;
  }
  // If button 3 is pressed on screen 3, decrease the value in HYS by 5
  else if (buttonsValue < 749 && buttonsValue > 650) { 
      HYSminus = HYS - 5;
      HYS = HYSminus;
  }   
}

if(screenState == 2) {
  // If button 3 is pressed on screen 2, increase the value in setPoint by 5
  if (buttonsValue < 800 && buttonsValue > 750) {
    setPointplus = setPoint + 5;
    setPoint = setPointplus;
  } 
  // If button 3 is pressed on screen 2, decrease the value in setPoint by 5
  else if (buttonsValue < 749 && buttonsValue > 650) {
      setPointminus = setPoint - 5;
      setPoint = setPointminus;
  }   
}

// Switch the LCD display based on the screen state
screenSwitch ();
switch(screenState) { 
  case 3:  // On LCD, show the value of PV, CO and variable HYS
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PV: ");
    lcd.print(PV);
    lcd.setCursor(8, 0); 
    lcd.print("CO: ");
    lcd.print(CO);

    lcd.setCursor(0, 1); 
    lcd.print("HYS: ");
    lcd.print(HYS);
      break;
  
  case 2:   // On LCD, show the value of PV, CO and variable Setpoint
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PV: ");
    lcd.print(PV);
    lcd.setCursor(8, 0); 
    lcd.print("CO: ");
    lcd.print(CO);

    lcd.setCursor(0, 1); 
    lcd.print("SP: ");
    lcd.print(setPoint);
      break;

}

 
// Print SP, PV, CO and HYS to serial monitor for serial plotter
Serial.print("SP = ");
Serial.print(setPoint);
Serial.print("\t PV = ");
Serial.print(PV);
Serial.print("\t CO = ");
Serial.print(CO);
Serial.print("\t HYS = ");
Serial.print(HYS);
Serial.println();

 
}


void screenSwitch () {
  // Read the value of the buttons from the analog pin
  buttonsValue = analogRead(buttonsPin);

  // Switch the screen state based on the button value range

  // If button 2 is pressed, swtich the LCD screen to screen 2
  if (buttonsValue < 649 && buttonsValue > 425) {
    // Set the screen state to 2
    screenState = 2;
  }

// If button 1 is pressed, swtich the LCD screen to screen 3
  if (buttonsValue < 425 && buttonsValue >= 0) {
    // Set the screen state to 3
    screenState = 3;
  }
 

 
}

