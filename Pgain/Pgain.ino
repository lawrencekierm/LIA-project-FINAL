/* This code implements a control system with a proportional controller using LCD for 
user interface and feedback. The code reads sensor values, calculates the control output
based on the setpoint and feedback error, and adjusts the actuator accordingly. It also allows
the user to control the system by adjusting the SP and P Gain through 4 buttons connected to an
analog pin. The LCD displays relevant information such as PV, CO ,SP, pGain and Error.*/

#include <LiquidCrystal.h>

// Pin for the buttons
const int buttonsPin = A4;  // All four buttons are connected to one analogInput ( A4)
int buttonsValue = 0;  // declare button value that ranges from 0-1024
int screenState = 0;  // declare screen state


//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int backlightPin = 9; // Pin for controlling LCD backlight
int contrast = 70;    // Pin for controlling LCD backlight

float gainplus;  // declare added value for P Gain
float gainminus; // declare subtracted value for P Gain
int setPointplus; // declare added value for SP
int setPointminus; // declare subtracted value for SP

int sensor = A1;  // Setting the pin for the sensor
int actuator = 10;   // Select the pin for the LED
int PV = 0;     //  The process variable / LDR value
int setPoint = 0;   // declare Setpoint
int CO = 0;     // Control output value
int Error = 0;   // declare variable to store the error
float pGain = 1.0;    // declare the P Gain variable



void setup() {
  // Set up the actuator pin as an output
  pinMode(actuator, OUTPUT);

  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);

    // Set the backlight pin as an output
  pinMode(backlightPin, OUTPUT);

}

void loop() {
  // Set the backlight brightness using the contrast value
  analogWrite(backlightPin, contrast);

  // Read the analog sensor value (PV) between 0-1024
  PV = analogRead(sensor);
  
  // Calculate the control output (CO) based on the setpoint and feedback error
  Error = setPoint-PV;       // Error is SP - PV
  CO = (pGain*Error)/10;     // multiply the Error by the Pgain
                            // divide by ten for better results as Pgain can vary in low decimal value

  // Limit the control output within the valid range
  if (CO >255) {
    CO = 255;      // putting Cap on the max value to not exceed 255
  }
    if (CO <0) {
      CO = 0;       // putting Cap on the min value not to be less than 0
    }

  // Set the actuator output based on the control output
  analogWrite(actuator, CO);

// Handle button presses and update setpoint or P Gain
if(screenState == 3) {
  // If button 4 is pressed on screen 3, increase the value in P Gain by 0.01
  if (buttonsValue < 800 && buttonsValue > 750) {
    gainplus = pGain + 0.01;
    pGain = gainplus;
  } 
  // If button 4 is pressed on screen 3, decrease the value in P Gain by 0.01
  else if (buttonsValue < 749 && buttonsValue > 650) {
      gainminus = pGain - 0.01;
      pGain = gainminus;
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
  case 3:      // On the LCD, show the value of PV, CO, Error and variable pGain
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PV: ");
    lcd.print(PV);
    lcd.setCursor(8, 0); 
    lcd.print("CO: ");
    lcd.print(CO);

    lcd.setCursor(0, 1); 
    lcd.print("pG:");
    lcd.print(pGain);
    lcd.setCursor(8, 1); 
    lcd.print("Err: ");
    lcd.print(Error);
      break;
  
  case 2:    // On the LCD, show the value of PV, CO, Error and variable SP 
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
    lcd.setCursor(8, 1); 
    lcd.print("Err: ");
    lcd.print(Error);
      break;

}

// Print the values on serial Monitor to obtain garph on serial plotter  
  Serial.print(setPoint);
  Serial.print(",");
  Serial.print(PV);
  Serial.print(",");
  Serial.print(CO);
  Serial.print(",");
  Serial.print(pGain);
 
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
    //delay(100);
  }

  // If button 1 is pressed, swtich the LCD screen to screen 3
  if (buttonsValue < 425 && buttonsValue >= 0) {
    // Set the screen state to 3
    screenState = 3;
    //delay(100);
  }
 

 
}


