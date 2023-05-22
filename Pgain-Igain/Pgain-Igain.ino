/* This code controls a process based on a desired setpoint using a Proportional-Integral (PI)
control algorithm. It gathers analog data from a sensor to assess the process's current
state, adjusts the control output to minimize the error between the setpoint and process variable,
and displays relevant information on the LCD. This code allows the user to interact with the system
and adjust the contol behaviour by modifying the proportional gain or integral gain.*/

#include <LiquidCrystal.h>

const int buttonsPin = A4;  // All four buttons are connected to one analogInput ( A4)
int buttonsValue = 0;       // declare button value that ranges from 0-1024
int screenState = 0;        // declare screen state

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int backlightPin = 9;   // Pin for controlling LCD backlight
int contrast = 70;      // Pin for controlling LCD backlight

int setPoint = 0;   // declare Setpoint
float gainplus;     // declare added value for P Gain
float gainminus;    // declare subtracted value for P Gain
float iGplus;       // declare added value for I Gain
float iGminus;      // declare subtracted value for I Gain

int sensor = A1;    // Setting the pin for the sensor
int inputPin = A5;    // Potentiometer connected to A5 for inputValue between 0-1024
int actuator = 10;    // Select the pin for the LED
int PV = 0;           //  The process variable / LDR value
int CO = 0;           // Control output value
int Error = 0;        // declare variable to store the error
float pGain = 0.01;   // declare the Proportional Gain variable
float iGain = 0.01;   // declare the Integral Gain variable
int integralAction;   // declare the intergral action
unsigned long previousMillis = 0; // declare variable
const long interval = 100;  //declare the integral in which the arduino performs the control of the loop
unsigned long currentMillis;  // declare the variable to store the current millis value


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

// Read the input value between 0-1024
  setPoint = analogRead(inputPin);

// Read the current millis value
  currentMillis = millis();

if (currentMillis - previousMillis >= interval) {

  previousMillis = currentMillis;
  Error = setPoint-PV;                // Error is SP-PV
  integralAction = (integralAction + Error);    // the integral is the accumulation of the error
  CO = (pGain*Error)+(iGain*integralAction)/100;  // the controller output is P control + I control
                                                  // divide by 100 for better result in output as it ranges in low decimal value

  if (CO >255) {
    CO = 255;       // putting Cap on the max value to not exceed 255
  }

    if (CO<0) {
      CO = 0;       // putting Cap on the min value not to be less than 0
    }
  }
  
// Set the actuator output based on the control output
analogWrite(actuator, CO);

// Handle button presses and update setpoint or P Gai
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
  // If button 4 is pressed on screen 2, increase the value in I Gain by 0.01
  if (buttonsValue < 800 && buttonsValue > 750) {
    iGplus = iGain + 0.01;
    iGain = iGplus;
  } 
  // If button 4 is pressed on screen 2, decrease the value in I Gain by 0.01
  else if (buttonsValue < 749 && buttonsValue > 650) {
      iGminus = iGain - 0.01;
      iGain = iGminus;
  }   
}


screenSwitch ();
switch(screenState) {
  case 3:         // On the LCD, show the value of PV, CO, Error and variable P gain
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
  
  case 2:     // On the LCD, show the value of PV, CO, Error and variable I gain
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("PV: ");
    lcd.print(PV);
    lcd.setCursor(8, 0); 
    lcd.print("CO: ");
    lcd.print(CO);

    lcd.setCursor(0, 1); 
    lcd.print("iG:");
    lcd.print(iGain);
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
Serial.print(",");
Serial.print(iGain);
Serial.println();

delay(100);

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
