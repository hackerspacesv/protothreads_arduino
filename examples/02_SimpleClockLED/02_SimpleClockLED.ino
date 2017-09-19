// Include to use LCD display
//#include <LiquidCrystal.h>
// Include to show
#include <stdio.h>
#include <Servo.h>

Servo servo1; 

//const int rs = 13, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int led1 = 6, led2 = 7, led3 = 8;
int led1_val = 0, led2_val = 0, led3_val = 0;

char stringTime[9] = "00:00:00";

void setup() {
  // Intialize LCD screen
  //lcd.begin(16, 2);
  //lcd.print(stringTime);
  Serial.begin(9600);
  Serial.println(stringTime);
  
  // Initialize LED Outputs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Set output values for LEDs
  digitalWrite(led1, led1_val);
  digitalWrite(led2, led2_val);
  digitalWrite(led3, led3_val);

}

long elapsedSeconds = 0;

void loop() {
  // Calculate the elapsed time based on total seconds;
  int hours = elapsedSeconds/(60*60);
  int minutes = (elapsedSeconds-(hours*60*60))/60;
  int seconds = elapsedSeconds-(hours*60*60)-(minutes*60);

  // Format time to display
  sprintf(stringTime, "%02d:%02d:%02d", hours, minutes, seconds);

  // Display on LCD
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(stringTime);
  Serial.println(stringTime);
  Serial.println(stringTime);

  // Changes led1 state every second
  led1_val = led1_val^1;
  digitalWrite(led1, led1_val);
  if(led1_val) {
    Serial.println("LED1 ON");
  } else {
    Serial.println("LED1 OFF");
  }

  // Changes led2 state every 3 seconds
  if((elapsedSeconds%3) == 0) {
    led2_val = led2_val^1;
    digitalWrite(led2, led2_val);
      
    if(led2_val) {
      Serial.println("LED2 ON");
    } else {
      Serial.println("LED2 OFF");
    }
  }

  // Changes led3 state every 5 seconds
  if((elapsedSeconds%5) == 0) {
    led3_val = led3_val^1;
    digitalWrite(led3, led3_val);
      
    if(led3_val) {
      Serial.println("LED3 ON");
    } else {
      Serial.println("LED3 OFF");
    }
  }

  // Increase second counter
  elapsedSeconds++;
  delay(1000);
}
